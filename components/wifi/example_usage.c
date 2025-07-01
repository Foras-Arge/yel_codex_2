#include "_wifi.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "u_nvs.h"
#include "_mqtt.h"

static const char *TAG = "MAIN";

static esp_mqtt_client_handle_t client = NULL;
static bool mqtt_is_connected = false; // MQTT bağlantı durumu

void app_main(void)
{
    u_nvs_init("wifi");

    // WiFi yapılandırması
    _wifi_config_t wifi_config = {
        .ssid = "Foras",             // WiFi ağınızın adı
        .password = "Foras_SleepAs", // WiFi şifreniz
        .max_retry = 5,              // Maksimum yeniden bağlanma denemesi
        .retry_interval_ms = 10000   // Yeniden bağlanma aralığı (10 saniye)
    };

    // WiFi kütüphanesini başlat (task otomatik olarak başlar)
    esp_err_t ret = _wifi_init_sta(&wifi_config);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "WiFi başlatılamadı: %s", esp_err_to_name(ret));
        return;
    }

    // WiFi bağlantısını başlat
    ret = _wifi_connect();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "WiFi bağlantısı başlatılamadı: %s", esp_err_to_name(ret));
        return;
    }

    ESP_LOGI(TAG, "WiFi kütüphanesi başlatıldı - MQTT devre dışı");
    bool mqtt_connected = false;
    int mqtt_retry_count = 0;
    const int max_mqtt_retries = 3;
    TickType_t last_mqtt_attempt = 0;
    const TickType_t mqtt_retry_delay = pdMS_TO_TICKS(30000); // 30 saniye

    // Ana döngü - diğer uygulama kodları buraya
    while (1)
    {
        TickType_t current_time = xTaskGetTickCount();

        // WiFi durumunu kontrol et
        if (_wifi_is_connected())
        {
            ESP_LOGI(TAG, "WiFi bağlantısı aktif");

            // WiFi durumunu al
            _wifi_status_t *status = _wifi_get_status();
            ESP_LOGI(TAG, "Bağlantı durumu: %s", status->is_connected ? "Bağlı" : "Bağlı değil");

            // MQTT bağlantısını başlat (retry limiti ve zaman aralığı ile)
            if (!mqtt_connected && client == NULL && mqtt_retry_count < max_mqtt_retries &&
                (current_time - last_mqtt_attempt) > mqtt_retry_delay)
            {
                ESP_LOGI(TAG, "MQTT bağlantısı başlatılıyor... (Deneme %d/%d)", mqtt_retry_count + 1, max_mqtt_retries);
                mqtt_init();
                mqtt_connected = true;
                mqtt_retry_count++;
                last_mqtt_attempt = current_time;
            }

            // MQTT bağlantı durumunu kontrol et
            if (mqtt_is_connected)
            {
                ESP_LOGI(TAG, "MQTT bağlantısı aktif - Test mesajı gönderiliyor...");
                mqtt_publish("esp32/status", "ESP32 çalışıyor!");
            }
        }
        else
        {
            ESP_LOGW(TAG, "WiFi bağlantısı yok");
            // WiFi bağlantısı kesilirse MQTT'yi de sıfırla
            if (mqtt_connected)
            {
                ESP_LOGI(TAG, "WiFi bağlantısı kesildi, MQTT sıfırlanıyor...");
                if (client != NULL)
                {
                    esp_mqtt_client_stop(client);
                    esp_mqtt_client_destroy(client);
                    client = NULL;
                }
                mqtt_connected = false;
                mqtt_is_connected = false;
                mqtt_retry_count = 0; // WiFi yeniden bağlandığında tekrar deneme
            }
        }

        // 10 saniye bekle
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}