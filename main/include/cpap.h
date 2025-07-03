#ifndef CPAP_H
#define CPAP_H

#include "_rtc.h"
#include "honeywell.h"
#include "string.h"
#include "u_math.h"
#include "motor_driver.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

typedef struct
{
    float pressure_setpoint;
    uint8_t motor_duty_percent;
    double motor_speed_array[6000];
    double motor_speed_average;
    double motor_speed_average_counter;
    uint8_t breath_state;
    bool easy_breath_status;
    float easy_breath_percent;
    uint16_t therapy_inspiration_count;
    uint16_t insp_counter_array[600];
    uint16_t minute_inspiration_count;
    float insp_duration;
    float insp_duration_average;
    float therapy_inspiration_duration;

    uint16_t therapy_expiration_count;
    uint16_t exp_counter_array[600];
    uint16_t minute_expiration_count;
    float exp_duration;
    float exp_duration_average;
    float therapy_expiration_duration;

    uint16_t respiration_rate;
    uint16_t therapy_respiration_rate;

    float no_breath_duration;

    bool easy_breath_task_suspended;
    bool pid_task_suspended;
    bool breath_detection_task_suspended;

    TaskHandle_t cpap_pid_control_task_handle;
    TaskHandle_t cpap_breath_detection_task_handle;
    TaskHandle_t cpap_easy_breath_task_handle;
    TaskHandle_t print_pressure_task_handle;
    TaskHandle_t cpap_average_motor_speed_task_handle;
    TaskHandle_t cpap_breath_conter_task_handle;
    TaskHandle_t cpap_breath_duration_task_handle;
} cpap;

extern cpap cpap_data;

void cpap_start();
void cpap_stop();

#endif