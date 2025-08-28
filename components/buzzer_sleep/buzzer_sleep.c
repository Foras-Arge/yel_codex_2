#include "buzzer_sleep.h"

TaskHandle_t sleep_task_handle = NULL, select_task_handle = NULL, buzzer_task_handle = NULL;
bool is_sleep = false;
uint8_t buzzer_counter = 0;

static void select_task(void *params)
{
    while (1)
    {
        if (gpio_get_level(SLEEP_PIN) == 0)
        {
            is_sleep = true;
            if (eTaskGetState(sleep_task_handle) == eSuspended)
            {
                vTaskResume(sleep_task_handle);
            }
        }
        else if (gpio_get_level(SLEEP_PIN) == 1)
        {
            is_sleep = false;
        }
        vTaskSuspend(NULL);
    }
}

static void sleep_task(void *params)
{
    vTaskSuspend(NULL);
    while (1)
    {
        vTaskResume(buzzer_task_handle);
        while (buzzer_counter != 10)
        {
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }

        printf("Sleeping...\n");
        vTaskDelay(100 / portTICK_PERIOD_MS);

        esp_sleep_enable_ext0_wakeup(SLEEP_PIN, 1);
        esp_deep_sleep_start();
    }
}

void IRAM_ATTR sleep_isr_handler(void *arg)
{
    xTaskResumeFromISR(select_task_handle);
}

static void buzzer_task(void *params)
{
    vTaskSuspend(NULL);
    while (1)
    {
        if (is_sleep)
        {
            gpio_set_level(BUZZER_PIN, 1);
            vTaskDelay(pdMS_TO_TICKS(1000));
            gpio_set_level(BUZZER_PIN, 0);
            vTaskDelay(pdMS_TO_TICKS(500));
            buzzer_counter++;
        }
        else
        {
            for (uint8_t i = 0; i < 10; i++)
            {
                gpio_set_level(BUZZER_PIN, 1);
                vTaskDelay(pdMS_TO_TICKS(100));
                gpio_set_level(BUZZER_PIN, 0);
                vTaskDelay(pdMS_TO_TICKS(50));
            }
            esp_restart();
        }
    }
}

void buzzer_sleep_init()
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BUZZER_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 0,
        .pull_down_en = 0,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);

    gpio_set_level(BUZZER_PIN, 0);

    gpio_config_t io_conf_sleep = {
        .pin_bit_mask = (1ULL << SLEEP_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = 0,
        .pull_down_en = 0,
        .intr_type = GPIO_INTR_ANYEDGE,
    };
    gpio_config(&io_conf_sleep);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(SLEEP_PIN, sleep_isr_handler, NULL);
    xTaskCreatePinnedToCore(sleep_task, "sleep_task", 4096, NULL, 20, &sleep_task_handle, 0);
    xTaskCreatePinnedToCore(buzzer_task, "buzzer_task", 4096, NULL, 20, &buzzer_task_handle, 0);
    xTaskCreatePinnedToCore(select_task, "select_task", 4096, NULL, 20, &select_task_handle, 0);
    vTaskDelay(200 / portTICK_PERIOD_MS);

    if (gpio_get_level(SLEEP_PIN) == 0)
    {
        if (eTaskGetState(select_task_handle) == eSuspended)
        {
            vTaskResume(select_task_handle);
        }
    }
}
