#include "_wifi.h"
#include "string.h"
#include "_mqtt.h"

static const char *TAG = "WIFI_STA";

// Global değişkenler
_wifi_config_t g_wifi_config;
static _wifi_status_t g_wifi_status = {0};
static int s_retry_num = 0;
static int s_max_retry = 5;
static TaskHandle_t wifi_task_handle = NULL;
static bool wifi_debug_enabled = false; // Debug varsayılan olarak aktif

// Debug kontrol fonksiyonları
void _wifi_set_debug(bool enabled)
{
    wifi_debug_enabled = enabled;
    if (enabled)
    {
        ESP_LOGI(TAG, "WiFi debug modu etkinleştirildi");
    }
    else
    {
        ESP_LOGI(TAG, "WiFi debug modu devre dışı bırakıldı");
    }
}

bool _wifi_get_debug_status(void)
{
    return wifi_debug_enabled;
}

// Event handler fonksiyonları
static void event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        ESP_LOGI(TAG, "WiFi STA başlatıldı, bağlantı kuruluyor...");
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        wifi_event_sta_disconnected_t *disconnect_info = (wifi_event_sta_disconnected_t *)event_data;
        g_wifi_status.is_connected = false;

        ESP_LOGW(TAG, "WiFi bağlantısı kesildi. Sebep: %d", disconnect_info->reason);

        if (s_retry_num < s_max_retry)
        {
            ESP_LOGI(TAG, "Yeniden bağlanmaya çalışılıyor... (%d/%d)", s_retry_num + 1, s_max_retry);
            esp_wifi_connect();
            s_retry_num++;
        }
        else
        {
            xEventGroupSetBits(g_wifi_status.wifi_event_group, WIFI_FAIL_BIT);
            ESP_LOGE(TAG, "Maksimum yeniden bağlanma denemesi aşıldı");
        }
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;

        // MAC adresini al
        uint8_t mac[6];
        esp_wifi_get_mac(WIFI_IF_STA, mac);

        ESP_LOGI(TAG, "WiFi bağlantısı başarılı!");
        ESP_LOGI(TAG, "MAC Adresi: %02X:%02X:%02X:%02X:%02X:%02X",
                 mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        ESP_LOGI(TAG, "IP Adresi: " IPSTR, IP2STR(&event->ip_info.ip));
        ESP_LOGI(TAG, "Gateway: " IPSTR, IP2STR(&event->ip_info.gw));
        ESP_LOGI(TAG, "Netmask: " IPSTR, IP2STR(&event->ip_info.netmask));

        s_retry_num = 0;
        g_wifi_status.is_connected = true;
        xEventGroupSetBits(g_wifi_status.wifi_event_group, WIFI_CONNECTED_BIT);
    }
}
// Yeniden bağlanma task'ı
static void wifi_reconnect_task(void *pvParameters)
{
    while (1)
    {
        if (!g_wifi_status.is_connected && g_wifi_status.is_initialized)
        {
            if (wifi_debug_enabled)
            {
                ESP_LOGI(TAG, "10 saniye sonra yeniden bağlanmaya çalışılacak...");
            }
            vTaskDelay(pdMS_TO_TICKS(10000)); // 10 saniye bekle

            if (!g_wifi_status.is_connected)
            {
                s_retry_num = 0;
                esp_wifi_connect();
                if (wifi_debug_enabled)
                {
                    ESP_LOGI(TAG, "Yeniden bağlanma denemesi başlatıldı");
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1 saniye bekle
    }
}

// Ana WiFi task'ı
static void wifi_main_task(void *pvParameters)
{
    if (wifi_debug_enabled)
    {
        ESP_LOGI(TAG, "WiFi ana task başlatıldı");
    }

    // Ana döngü - WiFi durumunu kontrol et
    while (1)
    {
        if (_wifi_is_connected())
        {
            if (wifi_debug_enabled)
            {
                ESP_LOGI(TAG, "WiFi bağlantısı aktif");

                // WiFi durumunu al
                _wifi_status_t *status = _wifi_get_status();
                ESP_LOGI(TAG, "Bağlantı durumu: %s", status->is_connected ? "Bağlı" : "Bağlı değil");
            }
        }
        else
        {
            if (wifi_debug_enabled)
            {
                ESP_LOGW(TAG, "WiFi bağlantısı yok, yeniden bağlanmaya çalışılıyor...");
            }
        }

        // 5 saniye bekle
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

esp_err_t _wifi_init_sta(_wifi_config_t *config)
{
    esp_err_t ret = ESP_OK;

    if (config == NULL)
    {
        if (wifi_debug_enabled)
        {
            ESP_LOGE(TAG, "WiFi yapılandırması NULL olamaz");
        }
        return ESP_ERR_INVALID_ARG;
    }

    // Yapılandırmayı kopyala
    memcpy(&g_wifi_config, config, sizeof(_wifi_config_t));
    s_max_retry = config->max_retry > 0 ? config->max_retry : 5;

    // Event group oluştur
    g_wifi_status.wifi_event_group = xEventGroupCreate();
    if (g_wifi_status.wifi_event_group == NULL)
    {
        if (wifi_debug_enabled)
        {
            ESP_LOGE(TAG, "Event group oluşturulamadı");
        }
        return ESP_FAIL;
    }

    // ESP-NETIF başlat
    ESP_ERROR_CHECK(esp_netif_init());

    // Event loop başlat
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Station netif oluştur
    esp_netif_create_default_wifi_sta();

    // WiFi başlat
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Event handler'ları kaydet
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        NULL));

    // WiFi yapılandırması
    wifi_config_t wifi_config = {
        .sta = {
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false},
            .failure_retry_cnt = 3,
            .listen_interval = 3,
        },
    };

    // SSID ve şifreyi kopyala
    strcpy((char *)wifi_config.sta.ssid, config->ssid);
    strcpy((char *)wifi_config.sta.password, config->password);

    ESP_LOGI(TAG, "WiFi yapılandırması: SSID=%s, Auth=%d", config->ssid, wifi_config.sta.threshold.authmode);

    // WiFi modunu ayarla
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));

    // Yeniden bağlanma task'ını başlat
    xTaskCreate(wifi_reconnect_task, "wifi_reconnect", 4096, NULL, 5, NULL);

    // Ana WiFi task'ını başlat
    xTaskCreate(wifi_main_task, "wifi_main", 4096, NULL, 5, &wifi_task_handle);

    g_wifi_status.is_initialized = true;
    g_wifi_status.is_connected = false;
    g_wifi_status.retry_count = 0;

    if (wifi_debug_enabled)
    {
        ESP_LOGI(TAG, "WiFi STA modu başlatıldı, SSID: %s", config->ssid);
    }

    return ret;
}

esp_err_t _wifi_connect(void)
{
    if (!g_wifi_status.is_initialized)
    {
        if (wifi_debug_enabled)
        {
            ESP_LOGE(TAG, "WiFi henüz başlatılmamış");
        }
        return ESP_FAIL;
    }

    esp_err_t ret = esp_wifi_start();
    if (ret == ESP_OK)
    {
        if (wifi_debug_enabled)
        {
            ESP_LOGI(TAG, "WiFi bağlantısı başlatıldı");
        }
    }
    else
    {
        if (wifi_debug_enabled)
        {
            ESP_LOGE(TAG, "WiFi başlatılamadı: %s", esp_err_to_name(ret));
        }
    }

    return ret;
}

esp_err_t _wifi_disconnect(void)
{
    if (!g_wifi_status.is_initialized)
    {
        if (wifi_debug_enabled)
        {
            ESP_LOGE(TAG, "WiFi henüz başlatılmamış");
        }
        return ESP_FAIL;
    }

    esp_err_t ret = esp_wifi_disconnect();
    if (ret == ESP_OK)
    {
        g_wifi_status.is_connected = false;
        if (wifi_debug_enabled)
        {
            ESP_LOGI(TAG, "WiFi bağlantısı kesildi");
        }
    }
    else
    {
        if (wifi_debug_enabled)
        {
            ESP_LOGE(TAG, "WiFi bağlantısı kesilemedi: %s", esp_err_to_name(ret));
        }
    }

    return ret;
}

bool _wifi_is_connected(void)
{
    return g_wifi_status.is_connected;
}

esp_err_t _wifi_deinit(void)
{
    if (!g_wifi_status.is_initialized)
    {
        if (wifi_debug_enabled)
        {
            ESP_LOGE(TAG, "WiFi henüz başlatılmamış");
        }
        return ESP_FAIL;
    }

    // Task'ları durdur
    if (wifi_task_handle != NULL)
    {
        vTaskDelete(wifi_task_handle);
        wifi_task_handle = NULL;
    }

    esp_err_t ret = esp_wifi_stop();
    if (ret == ESP_OK)
    {
        ret = esp_wifi_deinit();
        if (ret == ESP_OK)
        {
            g_wifi_status.is_initialized = false;
            g_wifi_status.is_connected = false;
            if (wifi_debug_enabled)
            {
                ESP_LOGI(TAG, "WiFi kütüphanesi durduruldu");
            }
        }
    }

    return ret;
}

_wifi_status_t *_wifi_get_status(void)
{
    return &g_wifi_status;
}

// SSID ve şifre değiştirme fonksiyonları
esp_err_t _wifi_set_ssid(const char *new_ssid)
{
    if (!g_wifi_status.is_initialized)
    {
        if (wifi_debug_enabled)
        {
            ESP_LOGE(TAG, "WiFi henüz başlatılmamış");
        }
        return ESP_FAIL;
    }

    if (new_ssid == NULL || strlen(new_ssid) == 0)
    {
        if (wifi_debug_enabled)
        {
            ESP_LOGE(TAG, "Geçersiz SSID");
        }
        return ESP_ERR_INVALID_ARG;
    }

    // Mevcut bağlantıyı kes
    esp_wifi_disconnect();

    // Yeni SSID'yi kopyala
    strncpy(g_wifi_config.ssid, new_ssid, sizeof(g_wifi_config.ssid) - 1);
    g_wifi_config.ssid[sizeof(g_wifi_config.ssid) - 1] = '\0';

    // WiFi yapılandırmasını güncelle
    wifi_config_t wifi_config = {
        .sta = {
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false},
        },
    };

    strcpy((char *)wifi_config.sta.ssid, g_wifi_config.ssid);
    strcpy((char *)wifi_config.sta.password, g_wifi_config.password);

    esp_err_t ret = esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    if (ret == ESP_OK)
    {
        if (wifi_debug_enabled)
        {
            ESP_LOGI(TAG, "SSID güncellendi: %s", new_ssid);
        }
        // Yeniden bağlan
        esp_wifi_connect();
    }
    else
    {
        if (wifi_debug_enabled)
        {
            ESP_LOGE(TAG, "SSID güncellenemedi: %s", esp_err_to_name(ret));
        }
    }

    return ret;
}

esp_err_t _wifi_set_password(const char *new_password)
{
    if (!g_wifi_status.is_initialized)
    {
        if (wifi_debug_enabled)
        {
            ESP_LOGE(TAG, "WiFi henüz başlatılmamış");
        }
        return ESP_FAIL;
    }

    if (new_password == NULL)
    {
        if (wifi_debug_enabled)
        {
            ESP_LOGE(TAG, "Geçersiz şifre");
        }
        return ESP_ERR_INVALID_ARG;
    }

    // Mevcut bağlantıyı kes
    esp_wifi_disconnect();

    // Yeni şifreyi kopyala
    strncpy(g_wifi_config.password, new_password, sizeof(g_wifi_config.password) - 1);
    g_wifi_config.password[sizeof(g_wifi_config.password) - 1] = '\0';

    // WiFi yapılandırmasını güncelle
    wifi_config_t wifi_config = {
        .sta = {
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false},
        },
    };

    strcpy((char *)wifi_config.sta.ssid, g_wifi_config.ssid);
    strcpy((char *)wifi_config.sta.password, g_wifi_config.password);

    esp_err_t ret = esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    if (ret == ESP_OK)
    {
        if (wifi_debug_enabled)
        {
            ESP_LOGI(TAG, "Şifre güncellendi");
        }
        // Yeniden bağlan
        esp_wifi_connect();
    }
    else
    {
        if (wifi_debug_enabled)
        {
            ESP_LOGE(TAG, "Şifre güncellenemedi: %s", esp_err_to_name(ret));
        }
    }

    return ret;
}

esp_err_t _wifi_set_credentials(const char *new_ssid, const char *new_password)
{
    if (!g_wifi_status.is_initialized)
    {
        if (wifi_debug_enabled)
        {
            ESP_LOGE(TAG, "WiFi henüz başlatılmamış");
        }
        return ESP_FAIL;
    }

    if (new_ssid == NULL || strlen(new_ssid) == 0)
    {
        if (wifi_debug_enabled)
        {
            ESP_LOGE(TAG, "Geçersiz SSID");
        }
        return ESP_ERR_INVALID_ARG;
    }

    if (new_password == NULL)
    {
        if (wifi_debug_enabled)
        {
            ESP_LOGE(TAG, "Geçersiz şifre");
        }
        return ESP_ERR_INVALID_ARG;
    }

    // Mevcut bağlantıyı kes
    esp_wifi_disconnect();

    // Yeni bilgileri kopyala
    strncpy(g_wifi_config.ssid, new_ssid, sizeof(g_wifi_config.ssid) - 1);
    g_wifi_config.ssid[sizeof(g_wifi_config.ssid) - 1] = '\0';

    strncpy(g_wifi_config.password, new_password, sizeof(g_wifi_config.password) - 1);
    g_wifi_config.password[sizeof(g_wifi_config.password) - 1] = '\0';

    // WiFi yapılandırmasını güncelle
    wifi_config_t wifi_config = {
        .sta = {
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false},
        },
    };

    strcpy((char *)wifi_config.sta.ssid, g_wifi_config.ssid);
    strcpy((char *)wifi_config.sta.password, g_wifi_config.password);

    esp_err_t ret = esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    if (ret == ESP_OK)
    {
        if (wifi_debug_enabled)
        {
            ESP_LOGI(TAG, "WiFi bilgileri güncellendi - SSID: %s", new_ssid);
        }
        // Yeniden bağlan
        esp_wifi_connect();
    }
    else
    {
        if (wifi_debug_enabled)
        {
            ESP_LOGE(TAG, "WiFi bilgileri güncellenemedi: %s", esp_err_to_name(ret));
        }
    }

    return ret;
}

// Mevcut SSID ve şifreyi alma fonksiyonları
const char *_wifi_get_ssid(void)
{
    return g_wifi_config.ssid;
}

const char *_wifi_get_password(void)
{
    return g_wifi_config.password;
}

void wifi_task(void *pvParameter)
{
    // WiFi yapılandırması

    strcpy(g_wifi_config.ssid, "Foras");             // WiFi ağınızın adı
    strcpy(g_wifi_config.password, "Foras_SleepAs"); // WiFi şifreniz
    g_wifi_config.max_retry = 5;                     // Maksimum yeniden bağlanma denemesi
    g_wifi_config.retry_interval_ms = 10000;         // Yeniden bağlanma aralığı (10 saniye)

    // WiFi kütüphanesini başlat (task otomatik olarak başlar)
    esp_err_t ret = _wifi_init_sta(&g_wifi_config);
    if (ret != ESP_OK)
    {
        if (wifi_debug_enabled)
        {
            ESP_LOGE(TAG, "WiFi başlatılamadı: %s", esp_err_to_name(ret));
        }
        return;
    }

    // WiFi bağlantısını başlat
    ret = _wifi_connect();
    if (ret != ESP_OK)
    {
        if (wifi_debug_enabled)
        {
            ESP_LOGE(TAG, "WiFi bağlantısı başlatılamadı: %s", esp_err_to_name(ret));
        }
        return;
    }

    if (wifi_debug_enabled)
    {
        ESP_LOGI(TAG, "WiFi kütüphanesi başlatıldı - MQTT devre dışı");
    }
    bool mqtt_connected = false;
    int mqtt_retry_count = 0;
    const int max_mqtt_retries = 3;
    TickType_t last_mqtt_attempt = 0;
    const TickType_t mqtt_retry_delay = pdMS_TO_TICKS(1000); // 1 saniye

    // Ana döngü - diğer uygulama kodları buraya
    while (1)
    {
        TickType_t current_time = xTaskGetTickCount();

        // WiFi durumunu kontrol et
        if (_wifi_is_connected())
        {
            if (wifi_debug_enabled)
            {
                ESP_LOGI(TAG, "WiFi bağlantısı aktif");

                // WiFi durumunu al
                _wifi_status_t *status = _wifi_get_status();
                ESP_LOGI(TAG, "Bağlantı durumu: %s", status->is_connected ? "Bağlı" : "Bağlı değil");
            }

            // MQTT bağlantısını başlat (retry limiti ve zaman aralığı ile)
            if (!mqtt_connected && mqtt_client == NULL && mqtt_retry_count < max_mqtt_retries &&
                (current_time - last_mqtt_attempt) > mqtt_retry_delay)
            {
                if (wifi_debug_enabled)
                {
                    ESP_LOGI(TAG, "MQTT bağlantısı başlatılıyor... (Deneme %d/%d)", mqtt_retry_count + 1, max_mqtt_retries);
                }
                mqtt_init();
                mqtt_connected = true;
                mqtt_retry_count++;
                last_mqtt_attempt = current_time;
            }
        }
        else
        {
            if (wifi_debug_enabled)
            {
                ESP_LOGW(TAG, "WiFi bağlantısı yok");
            }
            // WiFi bağlantısı kesilirse MQTT'yi de sıfırla
            if (mqtt_connected)
            {
                if (wifi_debug_enabled)
                {
                    ESP_LOGI(TAG, "WiFi bağlantısı kesildi, MQTT sıfırlanıyor...");
                }
                if (mqtt_client != NULL)
                {
                    esp_mqtt_client_stop(mqtt_client);
                    esp_mqtt_client_destroy(mqtt_client);
                    mqtt_client = NULL;
                }
                mqtt_connected = false;
                mqtt_retry_count = 0; // WiFi yeniden bağlandığında tekrar deneme
            }
        }
        // 10 saniye bekle
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void _wifi_init(void)
{
    xTaskCreate(wifi_task, "wifi_task", 8 * 1024, NULL, 5, NULL);
}