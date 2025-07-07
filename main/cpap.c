#include "cpap.h"
#include "esp_log.h"
#include <math.h>
#include "driver/gpio.h"
#include "esp_timer.h"

cpap cpap_data;

uint16_t insp_counter = 0;
uint16_t exp_counter = 0;
float now;
uint8_t old_breath_state = 3;
uint8_t apnea_type = 2;
bool settle_time_finished = false;
float insp_threshold = 0;
float exp_threshold = 0;
float target_pressure = 0;
static float insp_threshold_percent = 0;
static float exp_threshold_percent = 0;

void cpap_pid_control_task(void *params)
{
    int windup = 100;
    double Kp = 0.01, Ki = 0.00054, Kd = 0.0153;
    double error = 0;
    static double int_error = 0, old_error = 0;
    double motor_speed, bias;
    int16_t target_pressure = 0;
    cpap_data.motor_speed_average_counter = 0;
    while (1)
    {
        target_pressure = (int16_t)(cpap_data.pressure_setpoint * 10);
        bias = -4.3086e-06 * target_pressure * target_pressure + 0.0033225 * target_pressure + 0.1789;

        error = target_pressure - sensor_data.pressure;
        int_error += error;

        if (int_error > windup)
            int_error = windup;
        else if (int_error < -windup)
            int_error = -windup;

        motor_speed = Kp * error + Ki * int_error + Kd * (error - old_error) + bias;

        old_error = error;

        if (motor_speed <= 0.08)
        {
            motor_speed = 0.15;
        }
        else
        {
            if (motor_speed > 0.99)
                motor_speed = 0.99;
        }
        driver_speed_control((float)(motor_speed * 100));
        u_shift_and_insert_double(cpap_data.motor_speed_array, 6000, motor_speed * 100);
        if (cpap_data.motor_speed_average_counter > 5999)
        {
            cpap_data.motor_speed_average_counter = 6000;
        }
        else
        {
            cpap_data.motor_speed_average_counter++;
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void cpap_average_motor_speed_task(void *params)
{
    memset(cpap_data.motor_speed_array, 0, sizeof(cpap_data.motor_speed_array));
    cpap_data.motor_speed_average = 0;
    cpap_data.motor_speed_average_counter = 0;
    while (1)
    {
        cpap_data.motor_speed_average = u_average_double(cpap_data.motor_speed_array, cpap_data.motor_speed_average_counter);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void cpap_breath_detection_task(void *params)
{
    insp_threshold = cpap_data.pressure_setpoint * insp_threshold_percent / 100;
    exp_threshold = cpap_data.pressure_setpoint * exp_threshold_percent / 100;
    target_pressure = cpap_data.pressure_setpoint;
    cpap_data.breath_state = 0;
    now = 0;
    old_breath_state = 3;
    while (1)
    {
        float delta_pressure = sensor_data.real_pressure - target_pressure;

        if (cpap_data.breath_state != 1 && delta_pressure > insp_threshold)
        {
            old_breath_state = cpap_data.breath_state;
            cpap_data.breath_state = 1;
            vTaskResume(cpap_data.cpap_breath_duration_task_handle);
            exp_counter++;
            ESP_LOGI("BREATH", "Ekspirasyon (nefes verme) tespit edildi. Basınç: %.2f", sensor_data.real_pressure);
            if (cpap_data.easy_breath_status && cpap_data.easy_breath_task_suspended)
            {
                vTaskResume(cpap_data.cpap_easy_breath_task_handle);
                cpap_data.easy_breath_task_suspended = false;
            }
        }
        else if (cpap_data.breath_state != 2 && delta_pressure < exp_threshold)
        {
            old_breath_state = cpap_data.breath_state;
            cpap_data.breath_state = 2;
            vTaskResume(cpap_data.cpap_breath_duration_task_handle);
            insp_counter++;
            ESP_LOGI("BREATH", "İnspirasyon (nefes alma) tespit edildi. Basınç: %.2f", sensor_data.real_pressure);
        }

        if (cpap_data.breath_state != 0 && delta_pressure > exp_threshold && delta_pressure < insp_threshold)
        {
            ESP_LOGI("BREATH", "Nötr durumda bekle");
            old_breath_state = cpap_data.breath_state;
            cpap_data.breath_state = 0;
            vTaskResume(cpap_data.cpap_breath_duration_task_handle);
            if (settle_time_finished)
            {
                if (eTaskGetState(cpap_data.cpap_apnea_task_handle) == eSuspended)
                {
                    vTaskResume(cpap_data.cpap_apnea_task_handle);
                }
                if (eTaskGetState(cpap_data.cpap_obs_or_central_apnea_task_handle_start_task_handle) == eSuspended)
                {
                    vTaskResume(cpap_data.cpap_obs_or_central_apnea_task_handle_start_task_handle);
                }
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void cpap_obs_or_central_apnea_task_handle_start_task(void *params)
{
    vTaskSuspend(NULL);
    while (1)
    {
        if (cpap_data.breath_state != 0)
        {
            vTaskSuspend(NULL);
        }
        if (u_get_time_seconds() - now > 2)
        {
            if (eTaskGetState(cpap_data.cpap_obs_or_central_apnea_task_handle) == eSuspended)
            {
                vTaskResume(cpap_data.cpap_obs_or_central_apnea_task_handle);
            }
            vTaskSuspend(NULL);
        }
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void cpap_breath_duration_task(void *params)
{
    vTaskSuspend(NULL);
    now = u_get_time_seconds();
    cpap_data.hipoapnea_count = 0;
    cpap_data.obstructive_apnea_count = 0;
    cpap_data.central_apnea_count = 0;
    uint8_t hipoapnea_counter_ = 0;
    while (1)
    {
        if (old_breath_state == 1)
        {
            cpap_data.exp_duration = u_get_time_seconds() - now;
            cpap_data.therapy_expiration_duration += cpap_data.exp_duration;
            cpap_data.exp_duration_average = cpap_data.therapy_expiration_duration / cpap_data.therapy_expiration_count;
        }
        else if (old_breath_state == 2)
        {
            cpap_data.insp_duration = u_get_time_seconds() - now;
            if (settle_time_finished)
            {
                if (cpap_data.insp_duration > cpap_data.insp_duration_average * 1.4 || cpap_data.insp_duration < cpap_data.insp_duration_average * 0.4)
                {
                    cpap_data.therapy_inspiration_count--;
                    hipoapnea_counter_++;
                    if (hipoapnea_counter_ > 8)
                    {
                        cpap_data.hipoapnea_count++;
                        hipoapnea_counter_ = 0;
                    }
                }
                else
                {
                    cpap_data.therapy_inspiration_duration += cpap_data.insp_duration;
                    cpap_data.insp_duration_average = cpap_data.therapy_inspiration_duration / cpap_data.therapy_inspiration_count;
                    hipoapnea_counter_ = 0;
                }
            }
            else
            {
                cpap_data.therapy_inspiration_duration += cpap_data.insp_duration;
                cpap_data.insp_duration_average = cpap_data.therapy_inspiration_duration / cpap_data.therapy_inspiration_count;
                hipoapnea_counter_ = 0;
            }
        }
        else if (old_breath_state == 0)
        {
            cpap_data.no_breath_duration = u_get_time_seconds() - now;
        }
        now = u_get_time_seconds();
        vTaskSuspend(NULL);
    }
}

void cpap_apnea_task(void *params)
{
    vTaskSuspend(NULL);
    while (1)
    {
        if (u_get_time_seconds() - now > 10)
        {
            if (apnea_type == 0)
            {
                cpap_data.obstructive_apnea_count++;
            }
            else if (apnea_type == 1)
            {
                cpap_data.central_apnea_count++;
            }
            now = u_get_time_seconds();
        }
        if (cpap_data.breath_state != 0)
        {
            vTaskSuspend(NULL);
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void cpap_obs_or_central_apnea_task(void *params)
{
    vTaskSuspend(NULL);
    while (1)
    {
        if (sensor_data.real_pressure > cpap_data.pressure_setpoint * 0.98 && sensor_data.real_pressure < cpap_data.pressure_setpoint * 1.02)
        {
            printf("Central apnea\n");
            apnea_type = 1;
        }
        else
        {
            printf("Obstructive apnea\n");
            apnea_type = 0;
            vTaskSuspend(NULL);
        }

        if (cpap_data.breath_state != 0)
        {
            printf("Nefes var\n");
            vTaskSuspend(NULL);
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void print_pressure_task(void *params)
{
    while (1)
    {
        // printf("Basınç: %.2f, Hedef Basınç: %.2f, TInsp: %d, TExp: %d, TResp: %d, MinInsp: %d, MinExp: %d, MinResp: %d\n",
        //        sensor_data.real_pressure, cpap_data.pressure_setpoint, cpap_data.therapy_inspiration_count, cpap_data.therapy_expiration_count,
        //        cpap_data.therapy_respiration_rate, cpap_data.minute_inspiration_count, cpap_data.minute_expiration_count, cpap_data.respiration_rate);

        // printf("Süreler - İnsp: %.3fs, Exp: %.3fs, İnspOrt: %.3fs, ExpOrt: %.3fs, NefesYok: %.3fs\n",
        //        cpap_data.insp_duration, cpap_data.exp_duration, cpap_data.insp_duration_average,
        //        cpap_data.exp_duration_average, cpap_data.no_breath_duration);

        printf("Apnea - Hipo: %d, Obstr: %d, Centr: %d\n", cpap_data.hipoapnea_count, cpap_data.obstructive_apnea_count, cpap_data.central_apnea_count);

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void cpap_threshold_set_task(void *params)
{
    while (1)
    {
        if (target_pressure >= 3.0f && target_pressure < 8.0f)
        {
            insp_threshold_percent = 15.1;
            exp_threshold_percent = -20.1;
            insp_threshold = cpap_data.pressure_setpoint * insp_threshold_percent / 100;
            exp_threshold = cpap_data.pressure_setpoint * exp_threshold_percent / 100;
        }
        else if (target_pressure >= 8.0f && target_pressure < 10.0f)
        {
            insp_threshold_percent = 8.1;
            exp_threshold_percent = -10.1;
            insp_threshold = cpap_data.pressure_setpoint * insp_threshold_percent / 100;
            exp_threshold = cpap_data.pressure_setpoint * exp_threshold_percent / 100;
        }
        else if (target_pressure >= 10.0f && target_pressure < 14.0f)
        {
            insp_threshold_percent = 5.1;
            exp_threshold_percent = -7.14;
            insp_threshold = cpap_data.pressure_setpoint * insp_threshold_percent / 100;
            exp_threshold = cpap_data.pressure_setpoint * exp_threshold_percent / 100;
        }
        else if (target_pressure >= 14.0f && target_pressure < 17.0f)
        {
            insp_threshold_percent = 4.1;
            exp_threshold_percent = -6.14;
            insp_threshold = cpap_data.pressure_setpoint * insp_threshold_percent / 100;
            exp_threshold = cpap_data.pressure_setpoint * exp_threshold_percent / 100;
        }
        else if (target_pressure >= 17.0f && target_pressure <= 20.0f)
        {
            insp_threshold_percent = 3.5;
            exp_threshold_percent = -5.14;
            insp_threshold = cpap_data.pressure_setpoint * insp_threshold_percent / 100;
            exp_threshold = cpap_data.pressure_setpoint * exp_threshold_percent / 100;
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void cpap_easy_breath_task(void *params)
{
    double motor_speed = 0;
    bool task_active = false;

    vTaskSuspend(NULL);

    while (1)
    {
        if (!task_active)
        {
            // Task aktif değilse diğer task'ları suspend et
            if (!cpap_data.pid_task_suspended)
            {
                vTaskSuspend(cpap_data.cpap_pid_control_task_handle);
                cpap_data.pid_task_suspended = true;
            }
            if (!cpap_data.breath_detection_task_suspended)
            {
                vTaskSuspend(cpap_data.cpap_breath_detection_task_handle);
                cpap_data.breath_detection_task_suspended = true;
            }
            task_active = true;
        }

        motor_speed = cpap_data.motor_speed_average * (1 - cpap_data.easy_breath_percent / 100);
        driver_speed_control((float)(motor_speed));

        // Basınç hedefine ulaşana kadar bekle
        while (motor_speed < cpap_data.motor_speed_average && sensor_data.real_pressure < cpap_data.pressure_setpoint * 0.99)
        {
            motor_speed += 0.025;
            driver_speed_control((float)(motor_speed));
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }

        // Diğer task'ları resume et
        if (cpap_data.pid_task_suspended)
        {
            vTaskResume(cpap_data.cpap_pid_control_task_handle);
            cpap_data.pid_task_suspended = false;
        }
        if (cpap_data.breath_detection_task_suspended)
        {
            vTaskResume(cpap_data.cpap_breath_detection_task_handle);
            cpap_data.breath_detection_task_suspended = false;
        }

        task_active = false;
        cpap_data.easy_breath_task_suspended = true;
        vTaskSuspend(NULL);
    }
}

void cpap_breath_counter_task(void *params)
{
    cpap_data.therapy_inspiration_count = 0;
    cpap_data.therapy_expiration_count = 0;
    cpap_data.respiration_rate = 0;
    cpap_data.therapy_respiration_rate = 0;
    cpap_data.minute_inspiration_count = 0;
    cpap_data.minute_expiration_count = 0;
    insp_counter = 0;
    exp_counter = 0;

    // Array'leri sıfırla
    memset(cpap_data.insp_counter_array, 0, sizeof(cpap_data.insp_counter_array));
    memset(cpap_data.exp_counter_array, 0, sizeof(cpap_data.exp_counter_array));

    while (1)
    {
        u_shift_and_insert_uint16(cpap_data.insp_counter_array, 600, insp_counter);
        cpap_data.therapy_inspiration_count += insp_counter;
        insp_counter = 0;
        cpap_data.minute_inspiration_count = u_sum_uint16(cpap_data.insp_counter_array, 600);

        u_shift_and_insert_uint16(cpap_data.exp_counter_array, 600, exp_counter);
        cpap_data.therapy_expiration_count += exp_counter;
        exp_counter = 0;
        cpap_data.minute_expiration_count = u_sum_uint16(cpap_data.exp_counter_array, 600);

        cpap_data.respiration_rate = (cpap_data.minute_inspiration_count + cpap_data.minute_expiration_count) / 2;
        cpap_data.therapy_respiration_rate = (cpap_data.therapy_inspiration_count + cpap_data.therapy_expiration_count) / 2;

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void cpap_ramp_task(void *params)
{
    float old_setpoint = cpap_data.pressure_setpoint;
    cpap_data.pressure_setpoint = cpap_data.ramp_pressure;
    uint16_t ramp_counter = 1;
    while (1)
    {
        if (u_get_time_seconds() - cpap_data.start_time >= cpap_data.ramp_time * ramp_counter * 12)
        {
            cpap_data.pressure_setpoint = (((old_setpoint - cpap_data.ramp_pressure) / 5) * ramp_counter) + cpap_data.ramp_pressure;
            printf("Ramp: %.2f\n", cpap_data.pressure_setpoint);
            insp_threshold = cpap_data.pressure_setpoint * insp_threshold_percent / 100;
            exp_threshold = cpap_data.pressure_setpoint * exp_threshold_percent / 100;
            target_pressure = cpap_data.pressure_setpoint;
            ramp_counter++;
        }
        if (ramp_counter > 5)
        {
            settle_time_finished = true;
            vTaskDelete(NULL);
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void cpap_mask_off_task(void *params)
{
    bool timer_started = false;
    float timer_duration = 0;
    while (1)
    {
        if (sensor_data.real_pressure < target_pressure * 0.90)
        {
            if (!timer_started)
            {
                timer_duration = u_get_time_seconds();
                timer_started = true;
            }
        }
        else
        {
            if (timer_started)
            {
                timer_started = false;
                timer_duration = 0;
            }
        }

        if (timer_started)
        {
            if (u_get_time_seconds() - timer_duration > 5)
            {
                for (size_t i = 0; i < 10; i++)
                {
                    gpio_set_level(14, 1);
                    vTaskDelay(50 / portTICK_PERIOD_MS);
                    gpio_set_level(14, 0);
                    vTaskDelay(50 / portTICK_PERIOD_MS);
                }
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                cpap_stop();
            }
        }

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

void cpap_oto_start_task(void *params)
{
    cpap_data.oto_start_status = true;
    while (1)
    {
        if (cpap_data.oto_start_status)
        {
            if (sensor_data.real_pressure < -1.0f || sensor_data.real_pressure > 1.0f)
            {
                cpap_start();
                vTaskDelete(NULL);
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void cpap_init()
{
    xTaskCreate(cpap_oto_start_task, "CPAP OTO START", 6144, NULL, 5, &cpap_data.cpap_oto_start_task_handle);
}

void cpap_start()
{
    cpap_data.cpap_status = true;
    cpap_data.pressure_setpoint = 10.0f;

    cpap_data.easy_breath_status = true;
    cpap_data.easy_breath_percent = 50.0f;

    cpap_data.ramp_status = false;
    cpap_data.ramp_time = 5.0f;
    cpap_data.ramp_pressure = 5.0f;

    cpap_data.mask_off_status = true;

    cpap_data.start_time = u_get_time_seconds();

    cpap_data.easy_breath_task_suspended = false;
    cpap_data.pid_task_suspended = false;
    cpap_data.breath_detection_task_suspended = false;
    settle_time_finished = false;

    if (cpap_data.ramp_status)
    {
        xTaskCreate(cpap_ramp_task, "CPAP RAMP", 6144, NULL, 5, &cpap_data.cpap_ramp_task_handle);
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
    if (cpap_data.mask_off_status)
    {
        xTaskCreate(cpap_mask_off_task, "CPAP MASK OFF", 6144, NULL, 5, &cpap_data.cpap_mask_off_task_handle);
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
    xTaskCreate(cpap_threshold_set_task, "CPAP THRESHOLD SET", 6144, NULL, 5, &cpap_data.cpap_threshold_set_task_handle);
    xTaskCreate(cpap_pid_control_task, "CPAP", 6144, NULL, 5, &cpap_data.cpap_pid_control_task_handle);
    xTaskCreate(cpap_average_motor_speed_task, "CPAP AVERAGE MOTOR SPEED", 6144, NULL, 5, &cpap_data.cpap_average_motor_speed_task_handle);
    xTaskCreate(cpap_breath_counter_task, "CPAP BREATH COUNTER", 6144, NULL, 5, &cpap_data.cpap_breath_conter_task_handle);
    xTaskCreate(print_pressure_task, "PRINT PRESSURE", 6144, NULL, 5, &cpap_data.print_pressure_task_handle);
    xTaskCreate(cpap_breath_duration_task, "CPAP BREATH DURATION", 6144, NULL, 5, &cpap_data.cpap_breath_duration_task_handle);
    xTaskCreate(cpap_breath_detection_task, "CPAP BREATH DETECTION", 6144, NULL, 5, &cpap_data.cpap_breath_detection_task_handle);
    if (cpap_data.ramp_status)
    {
        while (!settle_time_finished)
        {
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }
    else
    {
        vTaskDelay(60000 / portTICK_PERIOD_MS);
        settle_time_finished = true;
    }
    xTaskCreate(cpap_obs_or_central_apnea_task_handle_start_task, "CPAP OBS OR CENTRAL APNEA TASK HANDLE START", 6144, NULL, 5, &cpap_data.cpap_obs_or_central_apnea_task_handle_start_task_handle);
    xTaskCreate(cpap_easy_breath_task, "CPAP EASY BREATH", 6144, NULL, 7, &cpap_data.cpap_easy_breath_task_handle);
    xTaskCreate(cpap_apnea_task, "CPAP APNEA", 6144, NULL, 5, &cpap_data.cpap_apnea_task_handle);
    xTaskCreate(cpap_obs_or_central_apnea_task, "CPAP OBS OR CENTRAL APNEA", 6144, NULL, 5, &cpap_data.cpap_obs_or_central_apnea_task_handle);
}

void cpap_stop()
{
    if (cpap_data.cpap_threshold_set_task_handle != NULL)
    {
        vTaskDelete(cpap_data.cpap_threshold_set_task_handle);
        cpap_data.cpap_threshold_set_task_handle = NULL;
    }
    if (cpap_data.cpap_pid_control_task_handle != NULL)
    {
        vTaskDelete(cpap_data.cpap_pid_control_task_handle);
        cpap_data.cpap_pid_control_task_handle = NULL;
    }
    if (cpap_data.cpap_breath_detection_task_handle != NULL)
    {
        vTaskDelete(cpap_data.cpap_breath_detection_task_handle);
        cpap_data.cpap_breath_detection_task_handle = NULL;
    }
    if (cpap_data.cpap_easy_breath_task_handle != NULL)
    {
        vTaskDelete(cpap_data.cpap_easy_breath_task_handle);
        cpap_data.cpap_easy_breath_task_handle = NULL;
    }
    if (cpap_data.cpap_average_motor_speed_task_handle != NULL)
    {
        vTaskDelete(cpap_data.cpap_average_motor_speed_task_handle);
        cpap_data.cpap_average_motor_speed_task_handle = NULL;
    }
    if (cpap_data.cpap_breath_conter_task_handle != NULL)
    {
        vTaskDelete(cpap_data.cpap_breath_conter_task_handle);
        cpap_data.cpap_breath_conter_task_handle = NULL;
    }
    if (cpap_data.print_pressure_task_handle != NULL)
    {
        vTaskDelete(cpap_data.print_pressure_task_handle);
        cpap_data.print_pressure_task_handle = NULL;
    }
    if (cpap_data.cpap_breath_duration_task_handle != NULL)
    {
        vTaskDelete(cpap_data.cpap_breath_duration_task_handle);
        cpap_data.cpap_breath_duration_task_handle = NULL;
    }
    if (cpap_data.cpap_apnea_task_handle != NULL)
    {
        vTaskDelete(cpap_data.cpap_apnea_task_handle);
        cpap_data.cpap_apnea_task_handle = NULL;
    }
    if (cpap_data.cpap_obs_or_central_apnea_task_handle != NULL)
    {
        vTaskDelete(cpap_data.cpap_obs_or_central_apnea_task_handle);
        cpap_data.cpap_obs_or_central_apnea_task_handle = NULL;
    }
    if (cpap_data.cpap_obs_or_central_apnea_task_handle_start_task_handle != NULL)
    {
        vTaskDelete(cpap_data.cpap_obs_or_central_apnea_task_handle_start_task_handle);
        cpap_data.cpap_obs_or_central_apnea_task_handle_start_task_handle = NULL;
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
    driver_speed_control(10);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    driver_speed_control(0);
    cpap_data.stop_time = u_get_time_seconds();
    printf("CPAP Süresi: %.2fs\n", cpap_data.stop_time - cpap_data.start_time);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    xTaskCreate(cpap_oto_start_task, "CPAP OTO START", 6144, NULL, 5, &cpap_data.cpap_oto_start_task_handle);
    if (cpap_data.cpap_mask_off_task_handle != NULL)
    {
        vTaskDelete(cpap_data.cpap_mask_off_task_handle);
        cpap_data.cpap_mask_off_task_handle = NULL;
    }
}
