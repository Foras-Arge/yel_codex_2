#include "_rtc.h"
#include "esp_log.h"

static i2c_master_soft_bus_handle_t i2c_bus = NULL;

// BCD'den decimal'e dönüştürme
static uint8_t bcd_to_dec(uint8_t bcd)
{
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

// Decimal'den BCD'ye dönüştürme
static uint8_t dec_to_bcd(uint8_t dec)
{
    return ((dec / 10) << 4) | (dec % 10);
}

_rtc_err_t _rtc_init(void)
{
    // I2C bus konfigürasyonu
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = RTC_SDA_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = RTC_SCL_PIN,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = RTC_I2C_FREQ};

    // Yazılım I2C bus oluşturma
    esp_err_t err = i2c_new_master_soft_bus(&conf, &i2c_bus);
    if (err != ESP_OK)
    {
#if CONFIG__rtc_DEBUG
        ESP_LOGE(TAG, "I2C bus oluşturulamadı!");
#endif
        return _RTC_ERROR_INIT;
    }

    // _rtc'yi başlat - Control1 register'ını temizle
    uint8_t control1 = 0x00;
    err = i2c_master_soft_bus_write_reg8(i2c_bus, _RTC_ADDR, _RTC_CONTROL1, 1, &control1);
    if (err != ESP_OK)
    {
#if CONFIG__rtc_DEBUG
        ESP_LOGE(TAG, "_rtc başlatılamadı!");
#endif
        return _RTC_ERROR_INIT;
    }

    return _RTC_OK;
}

_rtc_err_t _rtc_set_time(_rtc_time_t *time)
{
    if (time == NULL || i2c_bus == NULL)
    {
        return _RTC_ERROR_WRITE;
    }

    uint8_t time_data[7] = {
        dec_to_bcd(time->seconds) & 0x7F, // VL bit'ini temizle
        dec_to_bcd(time->minutes),
        dec_to_bcd(time->hours),
        dec_to_bcd(time->day),
        dec_to_bcd(time->weekday),
        dec_to_bcd(time->month),
        dec_to_bcd(time->year)};

    esp_err_t err = i2c_master_soft_bus_write_reg8(i2c_bus, _RTC_ADDR, _RTC_VL_SECONDS, 7, time_data);
    if (err != ESP_OK)
    {
#if CONFIG__rtc_DEBUG
        ESP_LOGE(TAG, "Saat ayarlanamadı!");
#endif
        return _RTC_ERROR_WRITE;
    }

    return _RTC_OK;
}

_rtc_err_t _rtc_get_time(_rtc_time_t *time)
{
    if (time == NULL || i2c_bus == NULL)
    {
        return _RTC_ERROR_READ;
    }

    uint8_t read_data[7];
    esp_err_t err = i2c_master_soft_bus_read_reg8(i2c_bus, _RTC_ADDR, _RTC_VL_SECONDS, 7, read_data);

    if (err != ESP_OK)
    {
#if CONFIG__rtc_DEBUG
        ESP_LOGE(TAG, "RTC okuma hatası!");
#endif
        return _RTC_ERROR_READ;
    }

    time->seconds = bcd_to_dec(read_data[0] & 0x7F); // VL bit'ini maskele
    time->minutes = bcd_to_dec(read_data[1] & 0x7F);
    time->hours = bcd_to_dec(read_data[2] & 0x3F); // 24 saat formatı
    time->day = bcd_to_dec(read_data[3] & 0x3F);
    time->weekday = bcd_to_dec(read_data[4] & 0x07);
    time->month = bcd_to_dec(read_data[5] & 0x1F);
    time->year = bcd_to_dec(read_data[6]);

    return _RTC_OK;
}

void _rtc_deinit(void)
{
    if (i2c_bus != NULL)
    {
        i2c_del_master_soft_bus(i2c_bus);
        i2c_bus = NULL;
    }
}

_rtc_err_t _rtc_set_time_from_string(const char *time_str)
{
    if (time_str == NULL || i2c_bus == NULL)
    {
        return _RTC_ERROR_WRITE;
    }

    int hours, minutes;
    if (sscanf(time_str, "%d,%d", &hours, &minutes) != 2)
    {
#if CONFIG__rtc_DEBUG
        ESP_LOGE(TAG, "Geçersiz saat formatı! Beklenen format: hh,mm");
#endif
        return _RTC_ERROR_WRITE;
    }

    // Saat ve dakika değerlerinin geçerliliğini kontrol et
    if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59)
    {
#if CONFIG__rtc_DEBUG
        ESP_LOGE(TAG, "Geçersiz saat veya dakika değeri!");
#endif
        return _RTC_ERROR_WRITE;
    }

    // Mevcut tarihi al
    _rtc_time_t current_time;
    _rtc_err_t err = _rtc_get_time(&current_time);
    if (err != _RTC_OK)
    {
        return err;
    }

    // Yeni saat değerlerini ayarla
    if (hours < 3)
    {
        hours = hours + 21;
    }
    else
    {
        hours = hours - 3;
    }
    current_time.hours = hours;
    current_time.minutes = minutes;
    current_time.seconds = 0; // Saniyeyi sıfırla

    return _rtc_set_time(&current_time);
}

_rtc_err_t _rtc_set_date_from_string(const char *date_str)
{
    if (date_str == NULL || i2c_bus == NULL)
    {
        return _RTC_ERROR_WRITE;
    }

    int day, month, year;
    if (sscanf(date_str, "%d,%d,%d", &day, &month, &year) != 3)
    {
#if CONFIG__rtc_DEBUG
        ESP_LOGE(TAG, "Geçersiz tarih formatı! Beklenen format: dd,mm,yyyy");
#endif
        return _RTC_ERROR_WRITE;
    }

    // Tarih değerlerinin geçerliliğini kontrol et
    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 2000 || year > 2099)
    {
#if CONFIG__rtc_DEBUG
        ESP_LOGE(TAG, "Geçersiz tarih değeri!");
#endif
        return _RTC_ERROR_WRITE;
    }

    // Mevcut saati al
    _rtc_time_t current_time;
    _rtc_err_t err = _rtc_get_time(&current_time);
    if (err != _RTC_OK)
    {
        return err;
    }

    // Yeni tarih değerlerini ayarla
    current_time.day = day;
    current_time.month = month;
    current_time.year = year - 2000; // RTC'de yıl 2000'den başlar

    return _rtc_set_time(&current_time);
}

_rtc_err_t _rtc_get_formatted_time(_rtc_formatted_time_t *formatted_time, bool is_display)
{
    if (formatted_time == NULL || i2c_bus == NULL)
    {
        return _RTC_ERROR_READ;
    }

    // RTC'den zamanı al
    _rtc_time_t rtc_time;
    _rtc_err_t err = _rtc_get_time(&rtc_time);
    if (err != _RTC_OK)
    {
        return err;
    }

    // Saat değerini UTC+3'e göre ayarla
    uint8_t hour = (rtc_time.hours + 3) % 24;

    // Değerleri stringlere çevir
    snprintf(formatted_time->hour, sizeof(formatted_time->hour), "%02d", hour);
    snprintf(formatted_time->minute, sizeof(formatted_time->minute), "%02d", rtc_time.minutes);
    snprintf(formatted_time->day, sizeof(formatted_time->day), "%02d", rtc_time.day);
    snprintf(formatted_time->month, sizeof(formatted_time->month), "%02d", rtc_time.month);
    if (is_display)
    {
        snprintf(formatted_time->year, sizeof(formatted_time->year), "%02d", rtc_time.year);
    }
    else
    {
        snprintf(formatted_time->year, sizeof(formatted_time->year), "%04d", rtc_time.year + 2000);
    }

    // Formatlanmış zaman stringini oluştur
    snprintf(formatted_time->formatted_time, sizeof(formatted_time->formatted_time),
             "%s:%s %s.%s.%s",
             formatted_time->hour,
             formatted_time->minute,
             formatted_time->day,
             formatted_time->month,
             formatted_time->year);

    return _RTC_OK;
}

_rtc_err_t _rtc_get_day_string(char *day_str, size_t size)
{
    if (day_str == NULL || i2c_bus == NULL || size < 3)
    {
        return _RTC_ERROR_READ;
    }

    _rtc_time_t rtc_time;
    _rtc_err_t err = _rtc_get_time(&rtc_time);
    if (err != _RTC_OK)
    {
        return err;
    }

    snprintf(day_str, size, "%02d", rtc_time.day);
    return _RTC_OK;
}

_rtc_err_t _rtc_get_month_string(char *month_str, size_t size)
{
    if (month_str == NULL || i2c_bus == NULL || size < 3)
    {
        return _RTC_ERROR_READ;
    }

    _rtc_time_t rtc_time;
    _rtc_err_t err = _rtc_get_time(&rtc_time);
    if (err != _RTC_OK)
    {
        return err;
    }

    snprintf(month_str, size, "%02d", rtc_time.month);
    return _RTC_OK;
}

_rtc_err_t _rtc_get_year_string(char *year_str, size_t size)
{
    if (year_str == NULL || i2c_bus == NULL || size < 5)
    {
        return _RTC_ERROR_READ;
    }

    _rtc_time_t rtc_time;
    _rtc_err_t err = _rtc_get_time(&rtc_time);
    if (err != _RTC_OK)
    {
        return err;
    }

    snprintf(year_str, size, "%04d", rtc_time.year + 2000);
    return _RTC_OK;
}

time_t _rtc_get_epoch_time(void)
{
    if (i2c_bus == NULL)
    {
        return 0;
    }

    _rtc_time_t rtc_time;
    if (_rtc_get_time(&rtc_time) != _RTC_OK)
    {
        return 0;
    }

    struct tm timeinfo = {
        .tm_sec = rtc_time.seconds,
        .tm_min = rtc_time.minutes,
        .tm_hour = rtc_time.hours,
        .tm_mday = rtc_time.day,
        .tm_mon = rtc_time.month - 1,  // Ay 0'dan başlar
        .tm_year = rtc_time.year + 100 // Yıl 1900'den başlar
    };

    return mktime(&timeinfo);
}