#ifndef SCREEN_FW_H
#define SCREEN_FW_H

#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/uart.h"
#include "sd_card.h"

// Firmware update states
typedef enum
{
    FW_UPDATE_IDLE = 0,
    FW_UPDATE_IN_PROGRESS,
    FW_UPDATE_SUCCESS,
    FW_UPDATE_FAILED
} fw_update_state_t;

// Function declarations
esp_err_t screen_fw_init(void);
esp_err_t screen_fw_update_start(void);
esp_err_t screen_fw_update_stop(void);
fw_update_state_t screen_fw_get_state(void);
void screen_fw_task(void *pvParameters);

#endif // SCREEN_FW_H