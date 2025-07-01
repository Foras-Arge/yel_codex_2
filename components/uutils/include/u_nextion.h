#pragma once

#include <stdio.h>
#include <stdint.h>
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// Nextion Configuration from menuconfig
#define NEXTION_TX_CONFIG GPIO_NUM_43
#define NEXTION_RX_CONFIG GPIO_NUM_44
#define NEXTION_BAUD_RATE 115200
#define NEXTION_BUF_SIZE 1024
#define NEXTION_TASK_STACK_SIZE 4096
#define NEXTION_TASK_PRIORITY 20
#define NEXTION_UART_PORT 1
#define NEXTION_QUEUE_SIZE 10
#define NEXTION_TIMEOUT_MS 1000

// Debug configuration
#define NEXTION_DEBUG 1

typedef struct
{
    uint8_t action_type;
    char action_data[NEXTION_BUF_SIZE];
    TaskHandle_t nextion_data_maping_handle;
} nextion_data_t;

typedef enum
{
    NEXTION_ACTION_TYPE_START_STOP,
    NEXTION_ACTION_TYPE_RAMP_TIME,
    NEXTION_ACTION_TYPE_RAMP_PRESSURE,
    NEXTION_ACTION_TYPE_HUMID_LEVEL,
    NEXTION_ACTION_TYPE_EASY_BREATH_PERCENT,
    NEXTION_ACTION_TYPE_BRIGHTNESS,
    NEXTION_ACTION_TYPE_SCREEN_SLEEP_TIME,
    NEXTION_ACTION_TYPE_MASK_OFF_ALARM,
    NEXTION_ACTION_TYPE_OTO_RAMP_OFF,
} nextion_action_type_t;

// Nextion data buffer
extern nextion_data_t nextion_data;

// Function prototypes
void nextion_init(void);
void nextion_listen_task(void *params);
void nextion_send_command(const char *command);
void nextion_set_text(const char *component_id, const char *text);
void nextion_set_value(const char *component_id, int value);
