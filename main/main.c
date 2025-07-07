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
    // İlk kurulum kontrolü - hata durumunda varsayılan değerleri kullan
    esp_err_t ret = u_nvs_get_string("first_start", settings.nvs_data, sizeof(settings.nvs_data));
    if (ret == ESP_OK)
    {
        settings.first_start = (strcmp(settings.nvs_data, "NO") == 0) ? false : true;
    }
    else
    {
        settings.first_start = true; // İlk kurulum varsayılanı
    }
    memset(settings.nvs_data, 0, sizeof(settings.nvs_data));

    if (!settings.first_start) //? İLK KURULUM DEĞIL
    {
        // Her bir ayar için, eğer get işlemi başarısız olursa (yani anahtar yoksa), ilk kurulum değerini yaz
        if (u_nvs_get_uint32("lang", &settings.language) != ESP_OK)
        {
            settings.language = LANGUAGE;
            u_nvs_set_uint32("lang", LANGUAGE);
        }
        if (u_nvs_get_uint32("dev_mode", &settings.device_mode) != ESP_OK)
        {
            settings.device_mode = DEVICE_MODE;
            u_nvs_set_uint32("dev_mode", DEVICE_MODE);
        }
        if (u_nvs_get_uint32("therapy", &therapy_settings.therapy_status) != ESP_OK)
        {
            therapy_settings.therapy_status = THERAPY_STATUS;
            u_nvs_set_uint32("therapy", THERAPY_STATUS);
        }
        if (u_nvs_get_uint32("wifi", &settings.wifi_status) != ESP_OK)
        {
            settings.wifi_status = WIFI_STATUS;
            u_nvs_set_uint32("wifi", WIFI_STATUS);
        }
        if (settings.wifi_status)
        {
            if (u_nvs_get_string("ssid", settings.wifi_ssid, sizeof(settings.wifi_ssid)) != ESP_OK)
            {
                strcpy(settings.wifi_ssid, ""); // Varsayılan olarak boş bırak
                u_nvs_set_string("ssid", "");
            }
            if (u_nvs_get_string("pass", settings.wifi_password, sizeof(settings.wifi_password)) != ESP_OK)
            {
                strcpy(settings.wifi_password, ""); // Varsayılan olarak boş bırak
                u_nvs_set_string("pass", "");
            }
        }
        if (u_nvs_get_uint32("ramp", &settings.ramp_status) != ESP_OK)
        {
            settings.ramp_status = RAMP_STATUS;
            u_nvs_set_uint32("ramp", RAMP_STATUS);
        }
        if (u_nvs_get_uint32("ramp_t", &settings.ramp_time) != ESP_OK)
        {
            settings.ramp_time = RAMP_TIME;
            u_nvs_set_uint32("ramp_t", RAMP_TIME);
        }
        if (u_nvs_get_uint32("ramp_p", &settings.ramp_pressure) != ESP_OK)
        {
            settings.ramp_pressure = RAMP_PRESSURE;
            u_nvs_set_uint32("ramp_p", RAMP_PRESSURE);
        }
        if (u_nvs_get_uint32("humid", &settings.humid_status) != ESP_OK)
        {
            settings.humid_status = HUMID_STATUS;
            u_nvs_set_uint32("humid", HUMID_STATUS);
        }
        if (u_nvs_get_uint32("humid_l", &settings.humid_level) != ESP_OK)
        {
            settings.humid_level = HUMID_LEVEL;
            u_nvs_set_uint32("humid_l", HUMID_LEVEL);
        }
        if (u_nvs_get_uint32("easy_b", &settings.easy_breath_status) != ESP_OK)
        {
            settings.easy_breath_status = EASY_BREATH_STATUS;
            u_nvs_set_uint32("easy_b", EASY_BREATH_STATUS);
        }
        if (u_nvs_get_uint32("easy_p", &settings.easy_breath_percent) != ESP_OK)
        {
            settings.easy_breath_percent = EASY_BREATH_PERCENT;
            u_nvs_set_uint32("easy_p", EASY_BREATH_PERCENT);
        }
        if (u_nvs_get_uint32("bright", &settings.brightness) != ESP_OK)
        {
            settings.brightness = BRIGHTNESS;
            u_nvs_set_uint32("bright", BRIGHTNESS);
        }
        if (u_nvs_get_uint32("sleep_t", &settings.screen_sleep_time) != ESP_OK)
        {
            settings.screen_sleep_time = SCREEN_SLEEP_TIME;
            u_nvs_set_uint32("sleep_t", SCREEN_SLEEP_TIME);
        }
        if (u_nvs_get_uint32("mask_a", &settings.mask_off_alarm) != ESP_OK)
        {
            settings.mask_off_alarm = MASK_OFF_ALARM;
            u_nvs_set_uint32("mask_a", MASK_OFF_ALARM);
        }
        if (u_nvs_get_uint32("oto_r", &settings.oto_ramp_off) != ESP_OK)
        {
            settings.oto_ramp_off = OTO_RAMP_OFF;
            u_nvs_set_uint32("oto_r", OTO_RAMP_OFF);
        }
        if (u_nvs_get_uint32("oto_s", &settings.oto_start) != ESP_OK)
        {
            settings.oto_start = OTO_START;
            u_nvs_set_uint32("oto_s", OTO_START);
        }
    }
    else //? İLK KURULUM
    {
        u_nvs_set_string("first_start", "NO");
        u_nvs_set_uint32("lang", LANGUAGE);
        u_nvs_set_uint32("dev_mode", DEVICE_MODE);
        u_nvs_set_uint32("therapy", THERAPY_STATUS);
        u_nvs_set_uint32("wifi", WIFI_STATUS);
        u_nvs_set_uint32("ramp", RAMP_STATUS);
        u_nvs_set_uint32("ramp_t", RAMP_TIME);
        u_nvs_set_uint32("ramp_p", RAMP_PRESSURE);
        u_nvs_set_uint32("humid", HUMID_STATUS);
        u_nvs_set_uint32("humid_l", HUMID_LEVEL);
        u_nvs_set_uint32("easy_b", EASY_BREATH_STATUS);
        u_nvs_set_uint32("easy_p", EASY_BREATH_PERCENT);
        u_nvs_set_uint32("bright", BRIGHTNESS);
        u_nvs_set_uint32("sleep_t", SCREEN_SLEEP_TIME);
        u_nvs_set_uint32("mask_a", MASK_OFF_ALARM);
        u_nvs_set_uint32("oto_r", OTO_RAMP_OFF);
        u_nvs_set_uint32("oto_s", OTO_START);

        settings.first_start = true;
        settings.language = LANGUAGE;
        settings.device_mode = DEVICE_MODE;
        therapy_settings.therapy_status = THERAPY_STATUS;
        settings.wifi_status = WIFI_STATUS;
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

    // NVS değişikliklerini kaydet
    u_nvs_commit();
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
    ESP_LOGI(TAG, "Bileşenler başlatılıyor...");

    // NVS'yi başlat
    esp_err_t ret = u_nvs_init("settings");
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "NVS başlatılamadı: %s", esp_err_to_name(ret));
        return;
    }

    // Ayarları yükle
    nvs_get_data();

    // WiFi durumunu kontrol et
    if (settings.wifi_status)
    {
        ESP_LOGI(TAG, "WiFi etkin, bağlantı kuruluyor...");
        // WiFi'yi başlat
        _wifi_init();
    }
    else
    {
        ESP_LOGI(TAG, "WiFi devre dışı");
    }

    // Diğer bileşenler şimdilik devre dışı
    _rtc_init();
    buzzer_sleep_init();
    honeywell_init();
    driver_init();
    screen_fw_init();
    nextion_init();

    ESP_LOGI(TAG, "Bileşenler başlatıldı");
}

void app_main(void)
{
    init_components();

    switch (settings.device_mode)
    {
    case DEVICE_MODE_CPAP:
    {
        cpap_init();
        break;
    }
    case DEVICE_MODE_APAP:
    {
        // apap_init();
        break;
    }
    case DEVICE_MODE_BPAP:
    {
        // bpap_init();
        break;
    }
    case DEVICE_MODE_BPAP_ST:
    {
        // bpap_st_init();
        break;
    }
    case DEVICE_MODE_AUTO_BPAP:
    {
        // auto_bpap_init();
        break;
    }
    case DEVICE_MODE_AVAPS:
    {
        // avaps_init();
        break;
    }
    case DEVICE_MODE_ASV:
    {
        // asv_init();
        break;
    }
    default:
        break;
    }
}
