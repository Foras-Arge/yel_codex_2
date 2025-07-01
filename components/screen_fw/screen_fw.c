#include "screen_fw.h"
#include "u_nextion.h"
#include "esp_log.h"
#include "esp_timer.h"
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>

#define TAG "SCREEN_FW"
#define FW_UPDATE_CHUNK_SIZE 4096
#define FW_UPDATE_TIMEOUT_MS 2000
#define FW_UPDATE_RESPONSE_TIMEOUT_MS 1000

static fw_update_state_t fw_update_state = FW_UPDATE_IDLE;
static SemaphoreHandle_t fw_update_mutex = NULL;
static TaskHandle_t fw_update_task_handle = NULL;

// Initialize firmware update module
esp_err_t screen_fw_init(void)
{
    fw_update_mutex = xSemaphoreCreateMutex();
    if (fw_update_mutex == NULL)
    {
        ESP_LOGE(TAG, "Failed to create mutex");
        return ESP_FAIL;
    }
    return ESP_OK;
}

// Start firmware update process
esp_err_t screen_fw_update_start(void)
{
    if (xSemaphoreTake(fw_update_mutex, pdMS_TO_TICKS(1000)) != pdTRUE)
    {
        ESP_LOGE(TAG, "Failed to take mutex");
        return ESP_FAIL;
    }

    if (fw_update_state != FW_UPDATE_IDLE)
    {
        xSemaphoreGive(fw_update_mutex);
        return ESP_ERR_INVALID_STATE;
    }

    fw_update_state = FW_UPDATE_IN_PROGRESS;
    xSemaphoreGive(fw_update_mutex);

    // Create firmware update task
    xTaskCreatePinnedToCore(screen_fw_task, "fw_update_task", 8192, NULL, 5, &fw_update_task_handle, 1);

    return ESP_OK;
}

// Stop firmware update process
esp_err_t screen_fw_update_stop(void)
{
    if (xSemaphoreTake(fw_update_mutex, pdMS_TO_TICKS(1000)) != pdTRUE)
    {
        return ESP_FAIL;
    }

    if (fw_update_state == FW_UPDATE_IN_PROGRESS)
    {
        fw_update_state = FW_UPDATE_FAILED;
    }

    xSemaphoreGive(fw_update_mutex);
    return ESP_OK;
}

// Get current firmware update state
fw_update_state_t screen_fw_get_state(void)
{
    fw_update_state_t state;
    if (xSemaphoreTake(fw_update_mutex, pdMS_TO_TICKS(1000)) != pdTRUE)
    {
        return FW_UPDATE_FAILED;
    }
    state = fw_update_state;
    xSemaphoreGive(fw_update_mutex);
    return state;
}

// Send command to Nextion and wait for response
static esp_err_t send_command_and_wait(const char *command, uint8_t expected_response)
{
    char response[2] = {0};

    // Send command if not NULL
    if (command != NULL)
    {
        nextion_send_command(command);
    }

    // Wait for response
    int64_t start_time = esp_timer_get_time();
    while ((esp_timer_get_time() - start_time) < (FW_UPDATE_RESPONSE_TIMEOUT_MS * 1000))
    {
        if (uart_read_bytes(NEXTION_UART_PORT, (uint8_t *)response, 1, pdMS_TO_TICKS(100)) > 0)
        {
            if (response[0] == expected_response)
            {
                return ESP_OK;
            }
        }
    }

    return ESP_ERR_TIMEOUT;
}

// Main firmware update task
void screen_fw_task(void *pvParameters)
{
    FILE *f = NULL;
    char file_path[256] = {0};
    uint8_t buffer[FW_UPDATE_CHUNK_SIZE];
    size_t bytes_read;
    uint32_t total_bytes = 0;
    uint32_t file_size = 0;
    struct stat st;

    // Construct file path
    snprintf(file_path, sizeof(file_path), "/sdcard/screen_fw/firmware.tft");

    // Get file size
    if (stat(file_path, &st) != 0)
    {
        ESP_LOGE(TAG, "Failed to get file size for %s", file_path);
        fw_update_state = FW_UPDATE_FAILED;
        vTaskDelete(NULL);
        return;
    }
    file_size = st.st_size;
    ESP_LOGI(TAG, "Firmware file size: %lu bytes", (unsigned long)file_size);

    // Open file
    f = fopen(file_path, "rb");
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open firmware file: %s", file_path);
        fw_update_state = FW_UPDATE_FAILED;
        vTaskDelete(NULL);
        return;
    }

    // Reset Nextion display first
    ESP_LOGI(TAG, "Resetting Nextion display...");
    nextion_send_command("rest");
    vTaskDelay(pdMS_TO_TICKS(1000)); // Wait for reset

    // Clear UART buffer
    uart_flush(NEXTION_UART_PORT);
    vTaskDelay(pdMS_TO_TICKS(100));

    // Send update command with lower baud rate (115200)
    char update_cmd[64];
    snprintf(update_cmd, sizeof(update_cmd), "whmi-wri %lu,115200,0", (unsigned long)file_size);
    ESP_LOGI(TAG, "Sending update command: %s", update_cmd);

    if (send_command_and_wait(update_cmd, 0x05) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to send update command or receive acknowledgment");
        fclose(f);
        fw_update_state = FW_UPDATE_FAILED;
        vTaskDelete(NULL);
        return;
    }

    ESP_LOGI(TAG, "Update command acknowledged, starting firmware transfer");

    // Send firmware data in chunks
    while ((bytes_read = fread(buffer, 1, FW_UPDATE_CHUNK_SIZE, f)) > 0)
    {
        // Send chunk
        uart_write_bytes(NEXTION_UART_PORT, (const char *)buffer, bytes_read);
        ESP_LOGI(TAG, "Sent chunk of %d bytes", bytes_read);

        // Wait for acknowledgment
        if (send_command_and_wait(NULL, 0x05) != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to receive chunk acknowledgment");
            fclose(f);
            fw_update_state = FW_UPDATE_FAILED;
            vTaskDelete(NULL);
            return;
        }

        total_bytes += bytes_read;
        unsigned long progress = (total_bytes * 100) / file_size;
        ESP_LOGI(TAG, "Progress: %lu%%", progress);
        if (progress == 100)
        {
            vTaskDelay(pdMS_TO_TICKS(5000));
            delete_folder_recursive("/sdcard/screen_fw");
            ESP_LOGI(TAG, "Firmware update completed successfully");
            vTaskDelay(pdMS_TO_TICKS(5000));
            esp_restart();
        }
    }

    fclose(f);

    // Wait for final acknowledgment
    if (send_command_and_wait(NULL, 0x05) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to receive final acknowledgment");
        fw_update_state = FW_UPDATE_FAILED;
        vTaskDelete(NULL);
        return;
    }

    ESP_LOGI(TAG, "Firmware update completed successfully");
    fw_update_state = FW_UPDATE_SUCCESS;

    vTaskDelete(NULL);
}