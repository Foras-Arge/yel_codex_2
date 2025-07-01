#ifndef _WIFI_H
#define _WIFI_H

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#ifdef __cplusplus
extern "C"
{
#endif

// Debug kontrolü için makro
#define WIFI_DEBUG_ENABLED 1 // 1: Debug aktif, 0: Debug pasif

#if WIFI_DEBUG_ENABLED
#define WIFI_DEBUG_LOG(tag, format, ...) ESP_LOGI(tag, format, ##__VA_ARGS__)
#define WIFI_DEBUG_ERROR(tag, format, ...) ESP_LOGE(tag, format, ##__VA_ARGS__)
#define WIFI_DEBUG_WARN(tag, format, ...) ESP_LOGW(tag, format, ##__VA_ARGS__)
#else
#define WIFI_DEBUG_LOG(tag, format, ...)
#define WIFI_DEBUG_ERROR(tag, format, ...)
#define WIFI_DEBUG_WARN(tag, format, ...)
#endif

// WiFi bağlantı durumu için event group bitleri
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

    // WiFi yapılandırma yapısı
    typedef struct
    {
        char ssid[32];
        char password[64];
        int max_retry;
        int retry_interval_ms;
    } _wifi_config_t;
    extern _wifi_config_t g_wifi_config;

    // WiFi durumu yapısı
    typedef struct
    {
        bool is_initialized;
        bool is_connected;
        int retry_count;
        EventGroupHandle_t wifi_event_group;
    } _wifi_status_t;

    /**
     * @brief WiFi debug modunu etkinleştirir/devre dışı bırakır
     * @param enabled true: debug aktif, false: debug pasif
     */
    void _wifi_set_debug(bool enabled);

    /**
     * @brief WiFi debug durumunu alır
     * @return true: debug aktif, false: debug pasif
     */
    bool _wifi_get_debug_status(void);

    /**
     * @brief WiFi kütüphanesini başlatır
     * @param config WiFi yapılandırma parametreleri
     * @return ESP_OK başarılı, ESP_FAIL başarısız
     */
    esp_err_t _wifi_init_sta(_wifi_config_t *config);

    /**
     * @brief WiFi bağlantısını başlatır
     * @return ESP_OK başarılı, ESP_FAIL başarısız
     */
    esp_err_t _wifi_connect(void);

    /**
     * @brief WiFi bağlantısını keser
     * @return ESP_OK başarılı, ESP_FAIL başarısız
     */
    esp_err_t _wifi_disconnect(void);

    /**
     * @brief WiFi bağlantı durumunu kontrol eder
     * @return true bağlı, false bağlı değil
     */
    bool _wifi_is_connected(void);

    /**
     * @brief WiFi kütüphanesini durdurur
     * @return ESP_OK başarılı, ESP_FAIL başarısız
     */
    esp_err_t _wifi_deinit(void);

    /**
     * @brief WiFi durumunu alır
     * @return WiFi durum yapısı
     */
    _wifi_status_t *_wifi_get_status(void);

    /**
     * @brief SSID'yi değiştirir
     * @param new_ssid Yeni SSID
     * @return ESP_OK başarılı, ESP_FAIL başarısız
     */
    esp_err_t _wifi_set_ssid(const char *new_ssid);

    /**
     * @brief Şifreyi değiştirir
     * @param new_password Yeni şifre
     * @return ESP_OK başarılı, ESP_FAIL başarısız
     */
    esp_err_t _wifi_set_password(const char *new_password);

    /**
     * @brief SSID ve şifreyi birlikte değiştirir
     * @param new_ssid Yeni SSID
     * @param new_password Yeni şifre
     * @return ESP_OK başarılı, ESP_FAIL başarısız
     */
    esp_err_t _wifi_set_credentials(const char *new_ssid, const char *new_password);

    /**
     * @brief Mevcut SSID'yi alır
     * @return SSID string'i
     */
    const char *_wifi_get_ssid(void);

    /**
     * @brief Mevcut şifreyi alır
     * @return Şifre string'i
     */
    const char *_wifi_get_password(void);

    /**
     * @brief WiFi kütüphanesini başlatır
     */
    void _wifi_init(void);

#ifdef __cplusplus
}
#endif

#endif // _WIFI_H
