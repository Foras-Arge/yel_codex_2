#ifndef MAIN_H
#define MAIN_H

#include "stdbool.h"
#include "stdint.h"
#include "driver/gpio.h"

//! İLK KURULUM DEĞERLERİ
#define WIFI_STATUS 1
#define LANGUAGE LANGUAGE_TURKISH
#define DEVICE_MODE DEVICE_MODE_CPAP
#define THERAPY_STATUS false
#define RAMP_STATUS false
#define RAMP_TIME 0
#define RAMP_PRESSURE 0
#define HUMID_STATUS false
#define HUMID_LEVEL 0
#define EASY_BREATH_STATUS false
#define EASY_BREATH_PERCENT 0
#define BRIGHTNESS 0
#define SCREEN_SLEEP_TIME 0
#define MASK_OFF_ALARM 0
#define OTO_RAMP_OFF 0
#define OTO_START 0
//! İLK KURULUM DEĞERLERİ

typedef enum
{
    DEVICE_MODE_CPAP,
    DEVICE_MODE_APAP,
    DEVICE_MODE_BPAP,
    DEVICE_MODE_BPAP_ST,
    DEVICE_MODE_AUTO_BPAP,
    DEVICE_MODE_AVAPS,
    DEVICE_MODE_ASV,
} device_mode_t;

typedef struct
{
    bool first_start;
    char nvs_data[1024];
    uint32_t wifi_status;
    char wifi_ssid[1024];
    char wifi_password[1024];
    uint32_t language;
    uint32_t device_mode;
    uint32_t ramp_status;
    uint32_t ramp_time;
    uint32_t ramp_pressure;
    uint32_t humid_status;
    uint32_t humid_level;
    uint32_t easy_breath_status;
    uint32_t easy_breath_percent;
    uint32_t brightness;
    uint32_t screen_sleep_time;
    uint32_t mask_off_alarm;
    uint32_t oto_ramp_off;
    uint32_t oto_start;
} settings_t;
extern settings_t settings;
typedef struct
{
    uint32_t therapy_status;
    uint16_t therapy_start_time;
    uint32_t therapy_end_time;
    char therapy_start_time_hmdmy[50];
    char therapy_end_time_hmdmy[50];
    uint32_t therapy_time;
    int32_t pressure;
    uint32_t pressure_min;
    uint32_t pressure_max;
    uint32_t flow;
    uint32_t tidal_volume;
    uint32_t inspiratory_time;
    uint32_t expiratory_time;
    uint32_t respiratory_rate;
    uint32_t inspiratory_flow;
    uint32_t expiratory_flow;
    uint32_t inspiratory_volume;
    uint32_t expiratory_volume;
    uint32_t peak_flow;
    uint32_t peak_flow_time;
    uint32_t peak_flow_volume;
    uint32_t peak_pressure;
    uint32_t peak_pressure_time;
    uint32_t peak_pressure_volume;
    uint32_t leak_flow;
    uint32_t leak_flow_volume;
    uint32_t ahi_index;
    uint32_t obs_count;
    uint32_t cent_count;
    uint32_t hipo_count;
} therapy_settings_t;
extern therapy_settings_t therapy_settings;

#endif
