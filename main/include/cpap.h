#ifndef CPAP_H
#define CPAP_H

#include "_rtc.h"
#include "honeywell.h"
#include "string.h"
#include "u_math.h"
#include "motor_driver.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

typedef struct
{
    bool cpap_status;
    bool oto_start_status;
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

    uint16_t hipoapnea_count;
    uint16_t obstructive_apnea_count;
    uint16_t central_apnea_count;

    TaskHandle_t cpap_pid_control_task_handle;
    TaskHandle_t cpap_breath_detection_task_handle;
    TaskHandle_t cpap_easy_breath_task_handle;
    TaskHandle_t print_pressure_task_handle;
    TaskHandle_t cpap_average_motor_speed_task_handle;
    TaskHandle_t cpap_breath_conter_task_handle;
    TaskHandle_t cpap_breath_duration_task_handle;
    TaskHandle_t cpap_apnea_task_handle;
    TaskHandle_t cpap_obs_or_central_apnea_task_handle;
    TaskHandle_t cpap_obs_or_central_apnea_task_handle_start_task_handle;
    TaskHandle_t cpap_ramp_task_handle;
    TaskHandle_t cpap_threshold_set_task_handle;
    TaskHandle_t cpap_mask_off_task_handle;
    TaskHandle_t cpap_oto_start_task_handle;
    TaskHandle_t cpap_start_task_handle;
    TaskHandle_t cpap_stop_task_handle;

    bool ramp_status;
    float ramp_time;
    float ramp_pressure;

    float start_time;
    float stop_time;

    bool mask_off_status;

} cpap;

extern cpap cpap_data;
void cpap_init();
void cpap_start(void *params);
void cpap_stop(void *params);

#endif