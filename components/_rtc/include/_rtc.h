#ifndef _RTC_H
#define _RTC_H
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "i2c_bus_soft.h"
#include "time.h"

// _RTC register adresleri
#define _RTC_ADDR 0x51
#define _RTC_CONTROL1 0x00
#define _RTC_CONTROL2 0x01
#define _RTC_VL_SECONDS 0x02
#define _RTC_MINUTES 0x03
#define _RTC_HOURS 0x04
#define _RTC_DAYS 0x05
#define _RTC_WEEKDAYS 0x06
#define _RTC_CENTURY_MONTHS 0x07
#define _RTC_YEARS 0x08
#define _RTC_MINUTE_ALARM 0x09
#define _RTC_HOUR_ALARM 0x0A
#define _RTC_DAY_ALARM 0x0B
#define _RTC_WEEKDAY_ALARM 0x0C
#define _RTC_CLKOUT_CONTROL 0x0D
#define _RTC_TIMER_CONTROL 0x0E
#define _RTC_TIMER 0x0F
#define RTC_I2C_FREQ 100000

#define RTC_SDA_PIN GPIO_NUM_18 //! RTC_SDA_PIN
#define RTC_SCL_PIN GPIO_NUM_17 //! RTC_SCL_PIN

// Zaman yapısı
typedef struct
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day;
    uint8_t weekday;
    uint8_t month;
    uint16_t year;
} _rtc_time_t;

// Hata kodları
typedef enum
{
    _RTC_OK = 0,
    _RTC_ERROR_INIT = -1,
    _RTC_ERROR_WRITE = -2,
    _RTC_ERROR_READ = -3
} _rtc_err_t;

// Zaman formatı için yapı
typedef struct
{
    char hour[4];
    char minute[4];
    char day[4];
    char month[4];
    char year[6];
    char formatted_time[50];
} _rtc_formatted_time_t;

_rtc_err_t _rtc_init(void);
_rtc_err_t _rtc_set_time(_rtc_time_t *time);
_rtc_err_t _rtc_set_time_from_string(const char *time_str); // Format: "hh,mm"
_rtc_err_t _rtc_set_date_from_string(const char *date_str); // Format: "dd,mm,yyyy"
_rtc_err_t _rtc_get_time(_rtc_time_t *time);
_rtc_err_t _rtc_get_formatted_time(_rtc_formatted_time_t *formatted_time, bool is_display);
_rtc_err_t _rtc_get_day_string(char *day_str, size_t size);
_rtc_err_t _rtc_get_month_string(char *month_str, size_t size);
_rtc_err_t _rtc_get_year_string(char *year_str, size_t size);
time_t _rtc_get_epoch_time(void);

#endif // _RTC_H_