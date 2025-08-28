#include "_mqtt.h"

static const char *TAG = "MQTT";

esp_mqtt_client_handle_t mqtt_client = NULL;
esp_mqtt_client_config_t mqtt_config = {0};
mqtt_data_t mqtt_data;
static bool mqtt_debug_enabled = MQTT_DEBUG_ENABLED; // Debug varsayılan olarak aktif

// Debug kontrol fonksiyonları
void mqtt_set_debug(bool enabled)
{
    mqtt_debug_enabled = enabled;
    if (enabled)
    {
        ESP_LOGI(TAG, "MQTT debug modu etkinleştirildi");
    }
    else
    {
        ESP_LOGI(TAG, "MQTT debug modu devre dışı bırakıldı");
    }
}

bool mqtt_get_debug_status(void)
{
    return mqtt_debug_enabled;
}

void mqtt_publish(const char *topic, const char *payload)
{
    if (mqtt_client == NULL)
    {
        if (mqtt_debug_enabled)
        {
            ESP_LOGE(TAG, "MQTT client NULL, mesaj gönderilemedi");
        }
        return;
    }

    if (topic == NULL || payload == NULL)
    {
        if (mqtt_debug_enabled)
        {
            ESP_LOGE(TAG, "Topic veya payload NULL, mesaj gönderilemedi");
        }
        return;
    }

    int msg_id = esp_mqtt_client_publish(mqtt_client, topic, payload, 0, 0, 0);
    if (msg_id < 0)
    {
        if (mqtt_debug_enabled)
        {
            ESP_LOGE(TAG, "MQTT mesaj gönderilemedi: %s", topic);
        }
    }
    else
    {
        if (mqtt_debug_enabled)
        {
            ESP_LOGI(TAG, "MQTT mesaj gönderildi: %s", topic);
        }
    }
}

void mqtt_subscribe(const char *topic)
{
    if (mqtt_client == NULL)
    {
        if (mqtt_debug_enabled)
        {
            ESP_LOGE(TAG, "MQTT client NULL, abone olunamadı");
        }
        return;
    }

    if (topic == NULL)
    {
        if (mqtt_debug_enabled)
        {
            ESP_LOGE(TAG, "Topic NULL, abone olunamadı");
        }
        return;
    }

    int msg_id = esp_mqtt_client_subscribe(mqtt_client, topic, 0);
    if (msg_id < 0)
    {
        if (mqtt_debug_enabled)
        {
            ESP_LOGE(TAG, "MQTT topic'e abone olunamadı: %s", topic);
        }
    }
    else
    {
        if (mqtt_debug_enabled)
        {
            ESP_LOGI(TAG, "MQTT topic'e abone olundu: %s", topic);
        }
    }
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;

    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:

        ESP_LOGI(TAG, "MQTT bağlantısı başarılı!");
        char topic[128];
        snprintf(topic, sizeof(topic), "Foras/yel/%s", SERIAL_NUMBER);
        mqtt_subscribe(topic);

        break;
    case MQTT_EVENT_DISCONNECTED:

        ESP_LOGI(TAG, "MQTT bağlantısı kesildi!");

        break;
    case MQTT_EVENT_SUBSCRIBED:

        ESP_LOGI(TAG, "MQTT topic'e abone olundu, msg_id=%d", event->msg_id);

        break;
    case MQTT_EVENT_UNSUBSCRIBED:

        ESP_LOGI(TAG, "MQTT topic aboneliği kaldırıldı, msg_id=%d", event->msg_id);

        break;
    case MQTT_EVENT_PUBLISHED:

        ESP_LOGI(TAG, "MQTT mesajı gönderildi, msg_id=%d", event->msg_id);

        break;
    case MQTT_EVENT_DATA:

        ESP_LOGI(TAG, "MQTT mesajı alındı:");
        ESP_LOGI(TAG, "Topic: %.*s", event->topic_len, event->topic);
        ESP_LOGI(TAG, "Data: %.*s", event->data_len, event->data);
        strncpy(mqtt_data.topic, event->topic, event->topic_len);
        strncpy(mqtt_data.data, event->data, event->data_len);
        // printf("mqtt data task : %d\n", eTaskGetState(mqtt_data.mqtt_data_mapping_task_handle));
        vTaskResume(mqtt_data.mqtt_data_mapping_task_handle);

        break;
    case MQTT_EVENT_ERROR:

        ESP_LOGE(TAG, "MQTT hatası, msg_id=%d", event->msg_id);

        break;
    default:

        ESP_LOGI(TAG, "MQTT diğer event id:%d", event->event_id);

        break;
    }
}

void mqtt_init(void)
{

    mqtt_config.broker.address.uri = "mqtt://192.168.2.72";
    mqtt_config.broker.address.port = 1883;
    mqtt_config.session.keepalive = 60;
    mqtt_config.session.disable_clean_session = 0;
    mqtt_config.session.last_will.qos = 1;
    mqtt_config.session.last_will.retain = 0;

    if (mqtt_debug_enabled)
    {
        ESP_LOGI(TAG, "MQTT başlatılıyor - Broker: 192.168.2.72:%d", 1883);
    }

    mqtt_client = esp_mqtt_client_init(&mqtt_config);
    if (mqtt_client == NULL)
    {
        if (mqtt_debug_enabled)
        {
            ESP_LOGE(TAG, "MQTT client oluşturulamadı!");
        }
        return;
    }

    esp_err_t ret = esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    if (ret != ESP_OK)
    {
        if (mqtt_debug_enabled)
        {
            ESP_LOGE(TAG, "MQTT event handler kaydedilemedi: %s", esp_err_to_name(ret));
        }
        esp_mqtt_client_destroy(mqtt_client);
        mqtt_client = NULL;
        return;
    }

    ret = esp_mqtt_client_start(mqtt_client);
    if (ret != ESP_OK)
    {
        if (mqtt_debug_enabled)
        {
            ESP_LOGE(TAG, "MQTT başlatılamadı: %s", esp_err_to_name(ret));
        }
        esp_mqtt_client_destroy(mqtt_client);
        mqtt_client = NULL;
        return;
    }

    if (mqtt_debug_enabled)
    {
        ESP_LOGI(TAG, "MQTT başlatıldı ve bağlantı bekleniyor...");
    }
}
