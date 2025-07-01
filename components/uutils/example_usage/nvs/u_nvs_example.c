#include "u_nvs.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"

static const char *TAG = "NVS_EXAMPLE";

// Ana uygulama
void app_main(void)
{
    ESP_LOGI(TAG, "Starting NVS example application...");

    // Initialize NVS
    esp_err_t ret = u_nvs_init("settings");
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize NVS: %s", esp_err_to_name(ret));
        return;
    }

    // Test string operations
    ret = u_nvs_set_string("test", "test");
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to set string: %s", esp_err_to_name(ret));
        return;
    }

    char test[10];
    ret = u_nvs_get_string("test", test, sizeof(test));
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to get string: %s", esp_err_to_name(ret));
        return;
    }

    printf("test: %s\n", test);

    // Test integer operations
    ret = u_nvs_set_int32("counter", 123);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to set int32: %s", esp_err_to_name(ret));
        return;
    }

    int32_t counter;
    ret = u_nvs_get_int32("counter", &counter);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to get int32: %s", esp_err_to_name(ret));
        return;
    }

    printf("counter: %ld\n", (long)counter);

    // Test uint32 operations
    ret = u_nvs_set_uint32("flags", 0xABCD);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to set uint32: %s", esp_err_to_name(ret));
        return;
    }

    uint32_t flags;
    ret = u_nvs_get_uint32("flags", &flags);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to get uint32: %s", esp_err_to_name(ret));
        return;
    }

    printf("flags: 0x%lX\n", (unsigned long)flags);

    // Commit changes
    ret = u_nvs_commit();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to commit: %s", esp_err_to_name(ret));
        return;
    }

    ESP_LOGI(TAG, "NVS test completed successfully");

    // Main loop
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(10000)); // 10 seconds
        ESP_LOGI(TAG, "NVS example running...");
    }
}