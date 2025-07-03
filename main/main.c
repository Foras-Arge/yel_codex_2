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
#include "cpap.h"

static const char *TAG = "MAIN";

settings_t settings;
therapy_settings_t therapy_settings;

// SD kart durum değişikliği callback fonksiyonu
void sd_card_status_callback(sd_card_status_t status, const char *message)
{
    switch (status)
    {
    case SD_CARD_INSERTED:
        ESP_LOGI(TAG, "✅ SD Kart Durumu: %s", message);
        break;

    case SD_CARD_REMOVED:
        ESP_LOGW(TAG, "❌ SD Kart Durumu: %s", message);
        break;

    case SD_CARD_ERROR:
        ESP_LOGE(TAG, "⚠️ SD Kart Durumu: %s", message);
        break;
    }
}

void nvs_get_data(void)
{
    u_nvs_get_string("first_start", settings.nvs_data, sizeof(settings.nvs_data)); //! İLK KURULUM MU DEĞIL MI
    settings.first_start = (strcmp(settings.nvs_data, "NO") == 0) ? false : true;
    memset(settings.nvs_data, 0, sizeof(settings.nvs_data));
    if (!settings.first_start) //? İLK KURULUM DEĞIL
    {
        // Her bir ayar için, eğer get işlemi başarısız olursa (yani anahtar yoksa), ilk kurulum değerini yaz
        if (u_nvs_get_uint32("language", &settings.language) != ESP_OK)
        {
            settings.language = LANGUAGE;
            u_nvs_set_uint32("language", LANGUAGE);
        }
        if (u_nvs_get_uint32("device_mode", &settings.device_mode) != ESP_OK)
        {
            settings.device_mode = DEVICE_MODE;
            u_nvs_set_uint32("device_mode", DEVICE_MODE);
        }
        if (u_nvs_get_uint32("therapy_status", &therapy_settings.therapy_status) != ESP_OK)
        {
            therapy_settings.therapy_status = THERAPY_STATUS;
            u_nvs_set_uint32("therapy_status", THERAPY_STATUS);
        }
        if (u_nvs_get_uint32("wifi_status", &settings.wifi_status) != ESP_OK)
        {
            settings.wifi_status = WIFI_STATUS_OFF;
            u_nvs_set_uint32("wifi_status", WIFI_STATUS_OFF);
        }
        if (settings.wifi_status)
        {
            if (u_nvs_get_string("wifi_ssid", settings.wifi_ssid, sizeof(settings.wifi_ssid)) != ESP_OK)
            {
                strcpy(settings.wifi_ssid, ""); // Varsayılan olarak boş bırak
                u_nvs_set_string("wifi_ssid", "");
            }
            if (u_nvs_get_string("wifi_password", settings.wifi_password, sizeof(settings.wifi_password)) != ESP_OK)
            {
                strcpy(settings.wifi_password, ""); // Varsayılan olarak boş bırak
                u_nvs_set_string("wifi_password", "");
            }
        }
        if (u_nvs_get_uint32("ramp_status", &settings.ramp_status) != ESP_OK)
        {
            settings.ramp_status = RAMP_STATUS;
            u_nvs_set_uint32("ramp_status", RAMP_STATUS);
        }
        if (u_nvs_get_uint32("ramp_time", &settings.ramp_time) != ESP_OK)
        {
            settings.ramp_time = RAMP_TIME;
            u_nvs_set_uint32("ramp_time", RAMP_TIME);
        }
        if (u_nvs_get_uint32("ramp_pressure", &settings.ramp_pressure) != ESP_OK)
        {
            settings.ramp_pressure = RAMP_PRESSURE;
            u_nvs_set_uint32("ramp_pressure", RAMP_PRESSURE);
        }
        if (u_nvs_get_uint32("humid_status", &settings.humid_status) != ESP_OK)
        {
            settings.humid_status = HUMID_STATUS;
            u_nvs_set_uint32("humid_status", HUMID_STATUS);
        }
        if (u_nvs_get_uint32("humid_level", &settings.humid_level) != ESP_OK)
        {
            settings.humid_level = HUMID_LEVEL;
            u_nvs_set_uint32("humid_level", HUMID_LEVEL);
        }
        if (u_nvs_get_uint32("easy_breath_status", &settings.easy_breath_status) != ESP_OK)
        {
            settings.easy_breath_status = EASY_BREATH_STATUS;
            u_nvs_set_uint32("easy_breath_status", EASY_BREATH_STATUS);
        }
        if (u_nvs_get_uint32("easy_breath_percent", &settings.easy_breath_percent) != ESP_OK)
        {
            settings.easy_breath_percent = EASY_BREATH_PERCENT;
            u_nvs_set_uint32("easy_breath_percent", EASY_BREATH_PERCENT);
        }
        if (u_nvs_get_uint32("brightness", &settings.brightness) != ESP_OK)
        {
            settings.brightness = BRIGHTNESS;
            u_nvs_set_uint32("brightness", BRIGHTNESS);
        }
        if (u_nvs_get_uint32("screen_sleep_time", &settings.screen_sleep_time) != ESP_OK)
        {
            settings.screen_sleep_time = SCREEN_SLEEP_TIME;
            u_nvs_set_uint32("screen_sleep_time", SCREEN_SLEEP_TIME);
        }
        if (u_nvs_get_uint32("mask_off_alarm", &settings.mask_off_alarm) != ESP_OK)
        {
            settings.mask_off_alarm = MASK_OFF_ALARM;
            u_nvs_set_uint32("mask_off_alarm", MASK_OFF_ALARM);
        }
        if (u_nvs_get_uint32("oto_ramp_off", &settings.oto_ramp_off) != ESP_OK)
        {
            settings.oto_ramp_off = OTO_RAMP_OFF;
            u_nvs_set_uint32("oto_ramp_off", OTO_RAMP_OFF);
        }
        if (u_nvs_get_uint32("oto_start", &settings.oto_start) != ESP_OK)
        {
            settings.oto_start = OTO_START;
            u_nvs_set_uint32("oto_start", OTO_START);
        }
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
    }
}

void sd_card_task(void *params)
{
    uint8_t sd_check_counter = 0;
    ESP_LOGI(TAG, "SD kart başlatılıyor...");
    esp_err_t sd_ret = sd_init_with_callback(sd_card_status_callback);
    if (sd_ret != ESP_OK)
    {
        ESP_LOGW(TAG, "SD kart başlatılamadı: %s", esp_err_to_name(sd_ret));
    }
    else
    {
        ESP_LOGI(TAG, "SD kart başarıyla başlatıldı");
    }

    while (1)
    {
        // Her 5 saniyede bir SD kart durumunu kontrol et
        if (sd_check_counter % 50 == 0)
        { // 50 * 100ms = 5 saniye
            // SD kart durumunu kontrol et (şimdilik sadece sayaç sıfırla)
            sd_check_counter = 0;
        }

        sd_check_counter++;
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void init_components(void)
{
    u_nvs_init("settings");
    nvs_get_data();
    _rtc_init();
    buzzer_sleep_init();
    honeywell_init();

    // Motor sürücüsünü geciktir
    vTaskDelay(pdMS_TO_TICKS(500));
    driver_init();

    screen_fw_init();
    nextion_init();

    // SD kart başlatmasını geçici olarak devre dışı bırak
    xTaskCreate(sd_card_task, "SD Card Task", 6144, NULL, 5, NULL);
    xTaskCreate(nextion_data_mapping_task, "Nextion Data Mapping Task", 4096, NULL, 5, &nextion_data.nextion_data_maping_handle);
    ESP_LOGI(TAG, "Tüm bileşenler başlatıldı (SD kart hariç)");

    if (!settings.first_start && settings.wifi_status)
    {
        _wifi_init();
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Uygulama başlatılıyor...");

    init_components();

    vTaskDelay(5000 / portTICK_PERIOD_MS);

    cpap_data.pressure_setpoint = 10.0f;

    cpap_start();
    vTaskDelay(300000 / portTICK_PERIOD_MS);
    cpap_stop();
}

// TODO
