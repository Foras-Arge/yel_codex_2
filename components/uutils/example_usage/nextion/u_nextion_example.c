#include "u_nextion.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"

static const char *TAG = "NEXTION_EXAMPLE";

typedef enum
{
    ACTION_TYPE_DENEME,
} action_type_t;

void nextion_data_mapping_task(void *params)
{
    while (1)
    {
        action_type_t action_type = (action_type_t)nextion_data.action_type;
        switch (action_type)
        {
        case ACTION_TYPE_DENEME:
        {
            printf("Action Type: %d, Action Data: %s\n", action_type, nextion_data.action_data);
        }
        break;
        default:
        {
            printf("data not found");
        }
        break;
        }
        vTaskSuspend(NULL);
    }
}

// Ana uygulama
void app_main(void)
{
    xTaskCreate(nextion_data_mapping_task, "nextion_data_mapping_task", 6144, NULL, 5, &nextion_data.nextion_data_maping_handle);
    nextion_init();
}