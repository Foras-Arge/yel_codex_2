#include "u_nextion.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include <string.h>
#include <stdio.h>

#define TAG "NEXTION"

nextion_data_t nextion_data;

// Global variables
char nextion_read_data[NEXTION_BUF_SIZE] = {0};
static QueueHandle_t uart_event_queue = NULL;
static SemaphoreHandle_t uart_tx_mutex = NULL;
static bool nextion_initialized = false;

// UART initialization function
static void uart_init(uart_port_t uart_num, int tx_pin, int rx_pin, int baudrate, int bufsize)
{
    uart_config_t uart_config = {
        .baud_rate = baudrate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    // Install UART driver
    esp_err_t ret = uart_driver_install(uart_num, bufsize * 2, bufsize * 2, NEXTION_QUEUE_SIZE, &uart_event_queue, 0);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to install UART driver: %s", esp_err_to_name(ret));
        return;
    }

    // Configure UART parameters
    ret = uart_param_config(uart_num, &uart_config);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to configure UART parameters: %s", esp_err_to_name(ret));
        uart_driver_delete(uart_num);
        return;
    }

    // Set UART pins
    ret = uart_set_pin(uart_num, tx_pin, rx_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to set UART pins: %s", esp_err_to_name(ret));
        uart_driver_delete(uart_num);
        return;
    }

    // Create mutex for TX safety
    uart_tx_mutex = xSemaphoreCreateMutex();
    if (uart_tx_mutex == NULL)
    {
        ESP_LOGE(TAG, "Failed to create UART TX mutex");
        uart_driver_delete(uart_num);
        return;
    }

    ESP_LOGI(TAG, "UART initialized: Port=%d, TX=%d, RX=%d, Baud=%d, Buffer=%d",
             uart_num, tx_pin, rx_pin, baudrate, bufsize);
}

// Receive data if it starts with '~'
static esp_err_t nextion_get_data(char *data, const uint8_t *buffer, int length)
{
    esp_err_t ret = ESP_FAIL;
    if (length > 0 && buffer[0] == '~' && buffer[1] == '[')
    {
        memcpy(data, buffer, length);
        data[length] = '\0';
        ret = ESP_OK;
    }
    return ret;
}

// UART RX event task
void nextion_uart_event_task(void *params)
{
    uart_event_t event;
    uint8_t buffer[NEXTION_BUF_SIZE];

    ESP_LOGI(TAG, "Nextion UART event task started");

    while (1)
    {
        if (xQueueReceive(uart_event_queue, &event, portMAX_DELAY))
        {
            bzero(buffer, sizeof(buffer));
            switch (event.type)
            {
            case UART_DATA:
                uart_read_bytes(NEXTION_UART_PORT, buffer, event.size, pdMS_TO_TICKS(100));
                if (nextion_get_data(nextion_read_data, buffer, event.size) == ESP_OK)
                {
                    if (nextion_data.nextion_data_maping_handle != NULL)
                    {
                        // Parse the received data format: ~[action_type]action_data
                        char *start_bracket = strchr(nextion_read_data, '[');
                        char *end_bracket = strchr(nextion_read_data, ']');

                        if (start_bracket && end_bracket && end_bracket > start_bracket)
                        {
                            // Extract action_type (number between brackets)
                            int action_type_len = end_bracket - start_bracket - 1;
                            char action_type_str[16];
                            strncpy(action_type_str, start_bracket + 1, action_type_len);
                            action_type_str[action_type_len] = '\0';

                            // Convert action_type string to integer
                            nextion_data.action_type = atoi(action_type_str);

                            // Extract action_data (everything after closing bracket)
                            char *action_data = end_bracket + 1;

                            strncpy(nextion_data.action_data, action_data, sizeof(nextion_data.action_data));
#if NEXTION_DEBUG
                            ESP_LOGI(TAG, "Parsed - Action Type: %d, Action Data: %s", nextion_data.action_type, nextion_data.action_data);
#endif
                            vTaskResume(nextion_data.nextion_data_maping_handle);
                        }
                    }
#if NEXTION_DEBUG
                    ESP_LOGI(TAG, "Received data: %s", nextion_read_data);
#endif
                }
                break;

            case UART_FIFO_OVF:
                ESP_LOGW(TAG, "UART FIFO overflow");
                uart_flush_input(NEXTION_UART_PORT);
                xQueueReset(uart_event_queue);
                break;

            case UART_BUFFER_FULL:
                ESP_LOGW(TAG, "UART buffer full");
                uart_flush_input(NEXTION_UART_PORT);
                xQueueReset(uart_event_queue);
                break;

            default:
                ESP_LOGW(TAG, "UART event type: %d", event.type);
                break;
            }
        }
    }
}

// Send command to Nextion
void nextion_send_command(const char *command)
{
    if (!nextion_initialized)
    {
        ESP_LOGE(TAG, "Nextion not initialized");
        return;
    }

    if (command == NULL)
    {
        ESP_LOGE(TAG, "Command is NULL");
        return;
    }

    const char end_cmd[3] = {0xFF, 0xFF, 0xFF};

    if (uart_tx_mutex && xSemaphoreTake(uart_tx_mutex, pdMS_TO_TICKS(NEXTION_TIMEOUT_MS)))
    {
        // Send end command first (Nextion protocol)
        uart_write_bytes(NEXTION_UART_PORT, end_cmd, 3);

        // Send the actual command
        uart_write_bytes(NEXTION_UART_PORT, command, strlen(command));

        // Send end command again
        uart_write_bytes(NEXTION_UART_PORT, end_cmd, 3);

        xSemaphoreGive(uart_tx_mutex);

#if NEXTION_DEBUG
        ESP_LOGI(TAG, "Command sent: %s", command);
#endif
    }
    else
    {
        ESP_LOGE(TAG, "Failed to take UART TX mutex");
    }

    // Small delay for Nextion processing
    vTaskDelay(pdMS_TO_TICKS(30));
}

// Set text component
void nextion_set_text(const char *component_id, const char *text)
{
    if (component_id == NULL || text == NULL)
    {
        ESP_LOGE(TAG, "Component ID or text is NULL");
        return;
    }

    char command[128];
    snprintf(command, sizeof(command), "%s.txt=\"%s\"", component_id, text);
    nextion_send_command(command);
}

// Set numeric value component
void nextion_set_value(const char *component_id, int value)
{
    if (component_id == NULL)
    {
        ESP_LOGE(TAG, "Component ID is NULL");
        return;
    }

    char command[128];
    snprintf(command, sizeof(command), "%s.val=%d", component_id, value);
    nextion_send_command(command);
}

// Init function
void nextion_init(void)
{
    if (nextion_initialized)
    {
        ESP_LOGW(TAG, "Nextion already initialized");
        return;
    }

    ESP_LOGI(TAG, "Initializing Nextion display...");

    // Initialize UART
    uart_init(NEXTION_UART_PORT, NEXTION_TX_CONFIG, NEXTION_RX_CONFIG, NEXTION_BAUD_RATE, NEXTION_BUF_SIZE);

    // Create UART event task
    BaseType_t task_created = xTaskCreate(nextion_uart_event_task, "nextion_uart_event_task",
                                          NEXTION_TASK_STACK_SIZE, NULL, NEXTION_TASK_PRIORITY, NULL);
    if (task_created != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to create Nextion UART event task");
        return;
    }

    nextion_initialized = true;
    ESP_LOGI(TAG, "Nextion display initialized successfully");
}

// Listen task (alias for compatibility)
void nextion_listen_task(void *params)
{
    nextion_uart_event_task(params);
}
