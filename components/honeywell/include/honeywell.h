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
    int raw_pressure;
    int raw_flow;
    int pressure;
    int flow;
    int pressure_base;
    int flow_base;
} sensor_data_t;

extern sensor_data_t sensor_data;

void honeywell_init();