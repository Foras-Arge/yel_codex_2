#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "_wifi.h"
#include "u_nvs.h"
#include "_rtc.h"
#include "buzzer_sleep.h"
#include "honeywell.h"
#include "motor_driver.h"
#include "sd_card.h"
#include "screen_fw.h"
#include "u_math.h"
#include "u_nvs.h"
#include "u_nextion.h"
#include "language.h"
#include "string.h"
#include "main.h"

settings_t settings;
therapy_settings_t therapy_settings;

void nvs_get_data(void)
{
    u_nvs_get_string("first_start", settings.nvs_data, sizeof(settings.nvs_data)); //! İLK KURULUM MU DEĞIL MI
    settings.first_start = (strcmp(settings.nvs_data, "NO") == 0) ? false : true;
    memset(settings.nvs_data, 0, sizeof(settings.nvs_data));
    if (!settings.first_start) //? İLK KURULUM DEĞIL
    {
        u_nvs_get_uint32("language", &settings.language);
        u_nvs_get_uint32("device_mode", &settings.device_mode);
        u_nvs_get_uint32("therapy_status", &therapy_settings.therapy_status);
        u_nvs_get_uint32("wifi_status", &settings.wifi_status);
        if (settings.wifi_status)
        {
            u_nvs_get_string("wifi_ssid", settings.wifi_ssid, sizeof(settings.wifi_ssid));
            u_nvs_get_string("wifi_password", settings.wifi_password, sizeof(settings.wifi_password));
        }
        u_nvs_get_uint32("ramp_status", &settings.ramp_status);
        u_nvs_get_uint32("ramp_time", &settings.ramp_time);
        u_nvs_get_uint32("ramp_pressure", &settings.ramp_pressure);
        u_nvs_get_uint32("humid_status", &settings.humid_status);
        u_nvs_get_uint32("humid_level", &settings.humid_level);
        u_nvs_get_uint32("easy_breath_status", &settings.easy_breath_status);
        u_nvs_get_uint32("easy_breath_percent", &settings.easy_breath_percent);
        u_nvs_get_uint32("brightness", &settings.brightness);
        u_nvs_get_uint32("screen_sleep_time", &settings.screen_sleep_time);
        u_nvs_get_uint32("mask_off_alarm", &settings.mask_off_alarm);
        u_nvs_get_uint32("oto_ramp_off", &settings.oto_ramp_off);
        u_nvs_get_uint32("oto_start", &settings.oto_start);
    }
    else //? İLK KURULUM
    {
        u_nvs_set_string("first_start", "NO");
        u_nvs_set_uint32("language", LANGUAGE);
        u_nvs_set_uint32("device_mode", DEVICE_MODE);
        u_nvs_set_uint32("therapy_status", THERAPY_STATUS);
        u_nvs_set_uint32("wifi_status", WIFI_STATUS_OFF);
        u_nvs_set_uint32("ramp_status", RAMP_STATUS);
        u_nvs_set_uint32("ramp_time", RAMP_TIME);
        u_nvs_set_uint32("ramp_pressure", RAMP_PRESSURE);
        u_nvs_set_uint32("humid_status", HUMID_STATUS);
        u_nvs_set_uint32("humid_level", HUMID_LEVEL);
        u_nvs_set_uint32("easy_breath_status", EASY_BREATH_STATUS);
        u_nvs_set_uint32("easy_breath_percent", EASY_BREATH_PERCENT);
        u_nvs_set_uint32("brightness", BRIGHTNESS);
        u_nvs_set_uint32("screen_sleep_time", SCREEN_SLEEP_TIME);
        u_nvs_set_uint32("mask_off_alarm", MASK_OFF_ALARM);
        u_nvs_set_uint32("oto_ramp_off", OTO_RAMP_OFF);
        u_nvs_set_uint32("oto_start", OTO_START);

        settings.first_start = true;
        settings.language = LANGUAGE;
        settings.device_mode = DEVICE_MODE;
        therapy_settings.therapy_status = THERAPY_STATUS;
        settings.wifi_status = WIFI_STATUS_OFF;
        settings.ramp_status = RAMP_STATUS;
        settings.ramp_time = RAMP_TIME;
        settings.ramp_pressure = RAMP_PRESSURE;
        settings.humid_status = HUMID_STATUS;
        settings.humid_level = HUMID_LEVEL;
        settings.easy_breath_status = EASY_BREATH_STATUS;
        settings.easy_breath_percent = EASY_BREATH_PERCENT;
        settings.brightness = BRIGHTNESS;
        settings.screen_sleep_time = SCREEN_SLEEP_TIME;
        settings.mask_off_alarm = MASK_OFF_ALARM;
        settings.oto_ramp_off = OTO_RAMP_OFF;
        settings.oto_start = OTO_START;
    } //! İLK KURULUM MU DEĞIL MI
}

void nextion_data_mapping_task(void *params)
{
    vTaskSuspend(NULL);
    while (1)
    {
        switch (nextion_data.action_type)
        {
        case NEXTION_ACTION_TYPE_START_STOP:
            //* İŞLEMLER
            break;
        case NEXTION_ACTION_TYPE_RAMP_TIME:
            //* İŞLEMLER
            break;
        case NEXTION_ACTION_TYPE_RAMP_PRESSURE:
            //* İŞLEMLER
            break;
        case NEXTION_ACTION_TYPE_HUMID_LEVEL:
            //* İŞLEMLER
            break;
        case NEXTION_ACTION_TYPE_EASY_BREATH_PERCENT:
            //* İŞLEMLER
            break;
        case NEXTION_ACTION_TYPE_BRIGHTNESS:
            //* İŞLEMLER
            break;
        case NEXTION_ACTION_TYPE_SCREEN_SLEEP_TIME:
            //* İŞLEMLER
            break;
        case NEXTION_ACTION_TYPE_MASK_OFF_ALARM:
            //* İŞLEMLER
            break;
        case NEXTION_ACTION_TYPE_OTO_RAMP_OFF:
            //* İŞLEMLER
            break;
        default:
            break;
        }
        vTaskResume(NULL);
    }
}

void init_components(void)
{
    u_nvs_init("settings");
    nvs_get_data();
    _rtc_init();
    sd_init();
    buzzer_sleep_init();
    honeywell_init();
    driver_init();
    screen_fw_init();
    nextion_init();
    if (settings.first_start)
    {
        //* İLK KURULUM
    }
    else
    {
        //* İLK KURULUM DEĞİL
    }
    //*HER TÜRLÜ YAPIALACAKLAR

    // _wifi_init();
}

void app_main(void)
{
    init_components();
    driver_speed_control(50.0f);

    while (1)
    {
        printf("fp_flow: %d, fp_pressure: %d\n", sensor_data.flow, sensor_data.pressure);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

// TODO
