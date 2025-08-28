#ifndef _MQTT_H
#define _MQTT_H

#include "esp_event.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#define SERIAL_NUMBER "YEL080725001"

#ifdef __cplusplus
extern "C"
{
#endif

// Debug kontrolü için makro
#define MQTT_DEBUG_ENABLED 0 // 1: Debug aktif, 0: Debug pasif

#if MQTT_DEBUG_ENABLED
#define MQTT_DEBUG_LOG(tag, format, ...) ESP_LOGI(tag, format, ##__VA_ARGS__)
#define MQTT_DEBUG_ERROR(tag, format, ...) ESP_LOGE(tag, format, ##__VA_ARGS__)
#define MQTT_DEBUG_WARN(tag, format, ...) ESP_LOGW(tag, format, ##__VA_ARGS__)
#else
#define MQTT_DEBUG_LOG(tag, format, ...)
#define MQTT_DEBUG_ERROR(tag, format, ...)
#define MQTT_DEBUG_WARN(tag, format, ...)
#endif

    typedef struct
    {
        char topic[128];
        char data[128];
        TaskHandle_t mqtt_data_mapping_task_handle;
    } mqtt_data_t;

    extern mqtt_data_t mqtt_data;

    extern esp_mqtt_client_handle_t mqtt_client;
    extern esp_mqtt_client_config_t mqtt_config;

    /**
     * @brief MQTT debug modunu etkinleştirir/devre dışı bırakır
     * @param enabled true: debug aktif, false: debug pasif
     */
    void mqtt_set_debug(bool enabled);

    /**
     * @brief MQTT debug durumunu alır
     * @return true: debug aktif, false: debug pasif
     */
    bool mqtt_get_debug_status(void);

    void mqtt_publish(const char *topic, const char *payload);
    void mqtt_subscribe(const char *topic);
    void mqtt_init(void);

#ifdef __cplusplus
}
#endif

#endif // _MQTT_H