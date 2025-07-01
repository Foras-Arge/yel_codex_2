#pragma once

#include <stdio.h>
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define FP_FLOW_SCL_IO GPIO_NUM_8  //! FP_FLOW_SCL_IO
#define FP_FLOW_SDA_IO GPIO_NUM_3  //! FP_FLOW_SDA_IO
#define FP_PRESS_SCL_IO GPIO_NUM_2 //! FP_PRESS_SCL_IO
#define FP_PRESS_SDA_IO GPIO_NUM_1 //! FP_PRESS_SDA_IO

typedef struct
{
    int16_t raw_pressure;
    int16_t raw_flow;
    int16_t pressure;
    int16_t flow;
    int16_t pressure_base;
    int16_t flow_base;
    float flow_array[60];
    uint16_t flow_counter;
    float real_flow;
    float real_pressure;
} sensor_data_t;

extern sensor_data_t sensor_data;

void honeywell_init();