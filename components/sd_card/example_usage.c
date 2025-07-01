#include "sd_card.h"
#include "esp_log.h"
#include <stdio.h>

static const char *TAG = "SD_EXAMPLE";

// SD kart durum değişikliği callback fonksiyonu
void sd_status_callback(sd_card_status_t status, const char *message)
{
    switch (status)
    {
    case SD_CARD_INSERTED:
        ESP_LOGI(TAG, "✅ SD Kart Durumu: %s", message);
        // Burada UI güncellemesi yapabilirsiniz
        // Örnek: nextion_set_value("page0.sd_status", 1);
        break;

    case SD_CARD_REMOVED:
        ESP_LOGW(TAG, "❌ SD Kart Durumu: %s", message);
        // Burada UI güncellemesi yapabilirsiniz
        // Örnek: nextion_set_value("page0.sd_status", 0);
        break;

    case SD_CARD_ERROR:
        ESP_LOGE(TAG, "⚠️ SD Kart Durumu: %s", message);
        // Burada UI güncellemesi yapabilirsiniz
        // Örnek: nextion_set_value("page0.sd_status", 2);
        break;
    }
}

void example_sd_card_usage(void)
{
    ESP_LOGI(TAG, "SD kart örnek kullanımı başlatılıyor...");

    // SD kartı callback ile başlat
    esp_err_t ret = sd_init_with_callback(sd_status_callback);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "SD kart başlatılamadı: %s", esp_err_to_name(ret));
        return;
    }

    // SD kart durumunu kontrol et
    sd_card_status_t status = sd_get_card_status();
    ESP_LOGI(TAG, "Mevcut SD kart durumu: %d", status);

    // SD kartın monte edilip edilmediğini kontrol et
    if (sd_is_card_mounted())
    {
        ESP_LOGI(TAG, "SD kart monte edilmiş ve kullanıma hazır");

        // Örnek dosya yazma
        ret = sd_write("test.txt", "Merhaba SD kart!");
        if (ret == ESP_OK)
        {
            ESP_LOGI(TAG, "Dosya başarıyla yazıldı");
        }
        else
        {
            ESP_LOGE(TAG, "Dosya yazma hatası: %s", esp_err_to_name(ret));
        }

        // Örnek dosya okuma
        char buffer[256];
        ret = sd_read("test.txt", buffer, sizeof(buffer));
        if (ret == ESP_OK)
        {
            ESP_LOGI(TAG, "Okunan veri: %s", buffer);
        }
        else
        {
            ESP_LOGE(TAG, "Dosya okuma hatası: %s", esp_err_to_name(ret));
        }
    }
    else
    {
        ESP_LOGW(TAG, "SD kart henüz monte edilmemiş");
    }
}

// SD kart durumunu sürekli kontrol eden örnek
void example_monitor_sd_status(void)
{
    while (1)
    {
        sd_card_status_t status = sd_get_card_status();
        bool is_mounted = sd_is_card_mounted();

        ESP_LOGI(TAG, "SD Durum: %d, Monte: %s",
                 status, is_mounted ? "Evet" : "Hayır");

        // Her 5 saniyede bir kontrol et
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

// SD kartı yeniden başlatma örneği
void example_force_reinit_sd(void)
{
    ESP_LOGI(TAG, "SD kartı yeniden başlatma denemesi...");

    esp_err_t ret = sd_force_reinit();
    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "SD kart başarıyla yeniden başlatıldı");
    }
    else
    {
        ESP_LOGE(TAG, "SD kart yeniden başlatılamadı: %s", esp_err_to_name(ret));
    }
}