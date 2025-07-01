# SD Kart Kütüphanesi

Bu kütüphane ESP32 için SD kart yönetimi sağlar. SD kartın takılması ve çıkarılması durumlarını otomatik olarak algılar ve yönetir.

## Özellikler

- ✅ SD kart takma/çıkarma otomatik algılama
- ✅ Durum değişikliği callback fonksiyonları
- ✅ Güvenli mount/unmount işlemleri
- ✅ Dosya okuma/yazma işlemleri
- ✅ OTA güncelleme desteği
- ✅ Hata durumu yönetimi
- ✅ Türkçe log mesajları

## Pin Konfigürasyonu

```c
#define VSPI_PIN_NUM_MISO 13  // MISO pini
#define VSPI_PIN_NUM_MOSI 11  // MOSI pini
#define VSPI_PIN_NUM_CLK  12  // Clock pini
#define VSPI_PIN_NUM_CS   10  // Chip Select pini
#define VSPI_PIN_NUM_DETECT 9 // Kart algılama pini
```

## Temel Kullanım

### 1. Basit Başlatma

```c
#include "sd_card.h"

void app_main(void)
{
    // SD kartı başlat
    esp_err_t ret = sd_init();
    if (ret != ESP_OK) {
        ESP_LOGE("MAIN", "SD kart başlatılamadı");
        return;
    }
    
    // SD kart hazır
    ESP_LOGI("MAIN", "SD kart başarıyla başlatıldı");
}
```

### 2. Callback ile Başlatma (Önerilen)

```c
#include "sd_card.h"

// SD kart durum değişikliği callback fonksiyonu
void sd_status_callback(sd_card_status_t status, const char* message)
{
    switch (status) {
        case SD_CARD_INSERTED:
            ESP_LOGI("SD", "✅ SD kart takıldı: %s", message);
            break;
        case SD_CARD_REMOVED:
            ESP_LOGW("SD", "❌ SD kart çıkarıldı: %s", message);
            break;
        case SD_CARD_ERROR:
            ESP_LOGE("SD", "⚠️ SD kart hatası: %s", message);
            break;
    }
}

void app_main(void)
{
    // SD kartı callback ile başlat
    esp_err_t ret = sd_init_with_callback(sd_status_callback);
    if (ret != ESP_OK) {
        ESP_LOGE("MAIN", "SD kart başlatılamadı");
        return;
    }
}
```

## API Referansı

### Temel Fonksiyonlar

#### `esp_err_t sd_init(void)`
SD kartı başlatır ve algılama görevini başlatır.

#### `esp_err_t sd_init_with_callback(sd_card_status_callback_t callback)`
SD kartı callback fonksiyonu ile başlatır.

#### `void sd_set_status_callback(sd_card_status_callback_t callback)`
Durum değişikliği callback fonksiyonunu ayarlar.

### Durum Kontrolü

#### `sd_card_status_t sd_get_card_status(void)`
Mevcut SD kart durumunu döndürür:
- `SD_CARD_REMOVED`: Kart çıkarılmış
- `SD_CARD_INSERTED`: Kart takılmış ve hazır
- `SD_CARD_ERROR`: Kart hatası

#### `bool sd_is_card_mounted(void)`
SD kartın monte edilip edilmediğini kontrol eder.

### Dosya İşlemleri

#### `esp_err_t sd_write(const char *filename, const char *data)`
Dosyaya veri yazar.

#### `esp_err_t sd_read(const char *filename, char *buffer, size_t buffer_size)`
Dosyadan veri okur.

#### `esp_err_t sd_append(const char *filename, const char *data)`
Dosyaya veri ekler.

#### `bool file_exists(const char *path)`
Dosyanın var olup olmadığını kontrol eder.

### Sistem Fonksiyonları

#### `esp_err_t sd_force_reinit(void)`
SD kartı zorla yeniden başlatır.

#### `void unmount_sd_card(void)`
SD kartı güvenli şekilde çıkarır.

#### `esp_err_t deinit_spi_bus(void)`
SPI bus'ı kapatır.

## Durum Yönetimi

### SD Kart Takıldığında
1. Kart algılama pini kontrol edilir
2. SPI bus başlatılır
3. SD kart mount edilir
4. `SD_CARD_INSERTED` durumu ayarlanır
5. Callback fonksiyonu çağrılır

### SD Kart Çıkarıldığında
1. Kart algılama pini kontrol edilir
2. SD kart güvenli şekilde unmount edilir
3. SPI bus kapatılır
4. `SD_CARD_REMOVED` durumu ayarlanır
5. Callback fonksiyonu çağrılır

### Hata Durumunda
1. Hata loglanır
2. `SD_CARD_ERROR` durumu ayarlanır
3. Callback fonksiyonu çağrılır
4. Yeniden deneme mekanizması devreye girer

## Örnek Kullanım Senaryoları

### 1. Veri Loglama

```c
void log_data_to_sd(const char* sensor_data)
{
    if (sd_is_card_mounted()) {
        char filename[64];
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        sprintf(filename, "logs/%04d%02d%02d.txt", 
                t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
        
        sd_append(filename, sensor_data);
    }
}
```

### 2. UI Güncelleme

```c
void sd_status_callback(sd_card_status_t status, const char* message)
{
    switch (status) {
        case SD_CARD_INSERTED:
            // UI'da yeşil LED yak
            gpio_set_level(LED_GREEN, 1);
            gpio_set_level(LED_RED, 0);
            break;
        case SD_CARD_REMOVED:
            // UI'da kırmızı LED yak
            gpio_set_level(LED_GREEN, 0);
            gpio_set_level(LED_RED, 1);
            break;
        case SD_CARD_ERROR:
            // UI'da sarı LED yak
            gpio_set_level(LED_GREEN, 0);
            gpio_set_level(LED_RED, 0);
            gpio_set_level(LED_YELLOW, 1);
            break;
    }
}
```

### 3. OTA Güncelleme Kontrolü

```c
void check_for_ota_update(void)
{
    if (sd_is_card_mounted()) {
        esp_err_t ret = check_and_perform_ota();
        if (ret == ESP_OK) {
            ESP_LOGI("OTA", "OTA güncellemesi bulundu ve başlatıldı");
        }
    }
}
```

## Hata Ayıklama

### Debug Modu
Debug mesajlarını görmek için `SD_DEBUG` makrosunu tanımlayın:

```c
#define SD_DEBUG
#include "sd_card.h"
```

### Log Seviyeleri
- `ESP_LOGI`: Bilgi mesajları
- `ESP_LOGW`: Uyarı mesajları  
- `ESP_LOGE`: Hata mesajları

## Güvenlik Notları

1. SD kart çıkarılmadan önce mutlaka unmount edilmelidir
2. Dosya işlemleri sırasında kart durumu kontrol edilmelidir
3. SPI bus paylaşımı dikkatli yönetilmelidir
4. Callback fonksiyonları kısa tutulmalıdır

## Sürüm Geçmişi

- **v2.0**: Callback sistemi ve gelişmiş durum yönetimi eklendi
- **v1.0**: Temel SD kart işlemleri

## Lisans

Bu kütüphane MIT lisansı altında lisanslanmıştır.
