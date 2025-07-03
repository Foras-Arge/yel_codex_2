#include "cpap.h"
#include "esp_log.h"
#include <math.h>
#include "driver/gpio.h"

cpap cpap_data;

uint16_t insp_counter = 0;
uint16_t exp_counter = 0;

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
    static float insp_threshold_percent = 4.1;
    static float exp_threshold_percent = -7.14;
    float insp_threshold = cpap_data.pressure_setpoint * insp_threshold_percent / 100;
    float exp_threshold = cpap_data.pressure_setpoint * exp_threshold_percent / 100;
    float target_pressure = cpap_data.pressure_setpoint;
    cpap_data.breath_state = 0;
    while (1)
    {
        float delta_pressure = sensor_data.real_pressure - target_pressure;

        if (cpap_data.breath_state != 1 && delta_pressure > insp_threshold)
        {
            cpap_data.breath_state = 1;
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
            cpap_data.breath_state = 2;
            insp_counter++;
            ESP_LOGI("BREATH", "İnspirasyon (nefes alma) tespit edildi. Basınç: %.2f", sensor_data.real_pressure);
        }

        if (cpap_data.breath_state != 0 && delta_pressure > exp_threshold && delta_pressure < insp_threshold)
        {
            ESP_LOGI("BREATH", "Nötr durumda bekle");
            cpap_data.breath_state = 0;
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void cpap_breath_duration_task(void *params)
{
}

void print_pressure_task(void *params)
{
    while (1)
    {
        // printf("Basınç: %.2f, TInsp: %d, TExp: %d, TResp: %d, MinInsp: %d, MinExp: %d, MinResp: %d\n",
        //        sensor_data.real_pressure, cpap_data.therapy_inspiration_count, cpap_data.therapy_expiration_count, cpap_data.therapy_respiration_rate,
        //        cpap_data.minute_inspiration_count, cpap_data.minute_expiration_count, cpap_data.respiration_rate);

        printf("Süreler - İnsp: %.3fs, Exp: %.3fs, İnspOrt: %.3fs, ExpOrt: %.3fs, NefesYok: %.3fs\n",
               cpap_data.insp_duration, cpap_data.exp_duration, cpap_data.insp_duration_average,
               cpap_data.exp_duration_average, cpap_data.no_breath_duration);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
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

void cpap_start()
{
    cpap_data.easy_breath_status = true;
    cpap_data.easy_breath_percent = 10.0f;

    // Task durum flag'lerini sıfırla
    cpap_data.easy_breath_task_suspended = false;
    cpap_data.pid_task_suspended = false;
    cpap_data.breath_detection_task_suspended = false;

    xTaskCreate(cpap_pid_control_task, "CPAP", 6144, NULL, 5, &cpap_data.cpap_pid_control_task_handle);
    xTaskCreate(cpap_average_motor_speed_task, "CPAP AVERAGE MOTOR SPEED", 6144, NULL, 5, &cpap_data.cpap_average_motor_speed_task_handle);
    xTaskCreate(cpap_breath_counter_task, "CPAP BREATH COUNTER", 6144, NULL, 5, &cpap_data.cpap_breath_conter_task_handle);
    xTaskCreate(print_pressure_task, "PRINT PRESSURE", 6144, NULL, 5, &cpap_data.print_pressure_task_handle);
    xTaskCreate(cpap_breath_duration_task, "CPAP BREATH DURATION", 6144, NULL, 5, &cpap_data.cpap_breath_duration_task_handle);
    vTaskDelay(10000 / portTICK_PERIOD_MS);
    xTaskCreate(cpap_breath_detection_task, "CPAP BREATH DETECTION", 6144, NULL, 5, &cpap_data.cpap_breath_detection_task_handle);
    xTaskCreate(cpap_easy_breath_task, "CPAP EASY BREATH", 6144, NULL, 7, &cpap_data.cpap_easy_breath_task_handle);
}

void cpap_stop()
{
    vTaskDelete(cpap_data.cpap_pid_control_task_handle);
    vTaskDelete(cpap_data.cpap_breath_detection_task_handle);
    vTaskDelete(cpap_data.cpap_easy_breath_task_handle);
    vTaskDelete(cpap_data.cpap_average_motor_speed_task_handle);
    vTaskDelete(cpap_data.cpap_breath_conter_task_handle);
    vTaskDelete(cpap_data.print_pressure_task_handle);
    vTaskDelete(cpap_data.cpap_breath_duration_task_handle);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    driver_speed_control(10);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    driver_speed_control(0);
}
