# UUtils - ESP32 Yardımcı Kütüphaneleri

Bu kütüphane ESP32/ESP8266 projeleri için geliştirilmiş çeşitli yardımcı fonksiyonları içeren bir kütüphanedir. Matematik işlemleri, NVS (Non-Volatile Storage) yönetimi ve Nextion ekran iletişimi için optimize edilmiş fonksiyonlar sağlar.

## 📋 İçindekiler

- [Genel Bakış](#genel-bakış)
- [Kurulum](#kurulum)
- [Kütüphane Bileşenleri](#kütüphane-bileşenleri)
- [Konfigürasyon](#konfigürasyon)
- [Kullanım Örnekleri](#kullanım-örnekleri)
- [API Referansı](#api-referansı)
- [Hata Yönetimi](#hata-yönetimi)
- [Performans](#performans)

## 🔍 Genel Bakış

UUtils kütüphanesi üç ana bileşenden oluşur:

### 🧮 **U_Math** - Matematik İşlemleri
- Array işlemleri (shift, insert, sum, average)
- Basınç-hız dönüşüm tabloları
- Hex string dönüşümleri
- String ayrıştırma fonksiyonları

### 💾 **U_NVS** - Kalıcı Depolama
- NVS (Non-Volatile Storage) wrapper fonksiyonları
- String, integer ve blob veri tipleri desteği
- Otomatik hata yönetimi ve loglama
- Thread-safe operasyonlar

### 📺 **U_Nextion** - Ekran İletişimi
- Nextion dokunmatik ekran iletişimi
- UART tabanlı komut gönderme/alma
- Asenkron veri işleme
- Event-driven mimari

## 🚀 Kurulum

### 1. Kütüphaneyi Projenize Ekleyin

```bash
# components/uutils dizinini projenize kopyalayın
cp -r components/uutils /path/to/your/project/components/
```

### 2. CMakeLists.txt'ye Ekleyin

```cmake
# Ana CMakeLists.txt dosyanızda
idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "main"
    REQUIRES uutils
)
```

### 3. Konfigürasyon

```bash
# Menuconfig ile ayarları yapılandırın
idf.py menuconfig
```

## 📚 Kütüphane Bileşenleri

### 🧮 U_Math Kütüphanesi

Matematiksel işlemler ve veri manipülasyonu için optimize edilmiş fonksiyonlar.

#### Özellikler:
- **Array İşlemleri**: Shift, insert, sum, average
- **Veri Tipleri**: float, int16_t, uint16_t, int64_t, uint64_t
- **Dönüşüm Fonksiyonları**: Basınç-hız, hex-bytes
- **String İşleme**: Parse ve ayrıştırma

#### Kullanım Örneği:
```c
#include "u_math.h"

// Array işlemleri
float values[10];
u_shift_and_insert_float(values, 10, 25.5);
float avg = u_average_float(values, 10);

// Basınç-hız dönüşümü
float speed = pressure_to_speed(50.0);

// Hex dönüşümü
size_t len;
uint8_t *bytes = u_hex_to_bytes("FF00AA", &len);
```

### 💾 U_NVS Kütüphanesi

ESP32'nin NVS sistemini basitleştiren wrapper fonksiyonları.

#### Özellikler:
- **Veri Tipleri**: String, Int32, Uint32, Blob
- **Otomatik Hata Yönetimi**: Kapsamlı hata kontrolü
- **Thread-Safe**: Güvenli çoklu thread kullanımı
- **Debug Desteği**: Detaylı loglama

#### Kullanım Örneği:
```c
#include "u_nvs.h"

// NVS başlatma
u_nvs_init("settings");

// Veri yazma
u_nvs_set_string("device_name", "ESP32_Device");
u_nvs_set_int32("counter", 123);
u_nvs_set_uint32("flags", 0xABCD);

// Veri okuma
char name[32];
u_nvs_get_string("device_name", name, sizeof(name));

// Değişiklikleri kaydetme
u_nvs_commit();
```

### 📺 U_Nextion Kütüphanesi

Nextion dokunmatik ekranlar ile iletişim için optimize edilmiş kütüphane.

#### Özellikler:
- **UART İletişimi**: Otomatik port yönetimi
- **Asenkron İşleme**: Ayrı task'ta veri dinleme
- **Komut Gönderme**: Text, value, command desteği
- **Event Handling**: Queue tabanlı mesaj işleme

#### Kullanım Örneği:
```c
#include "u_nextion.h"

// Nextion başlatma
nextion_init();

// Komut gönderme
nextion_send_command("page 1");
nextion_set_text("t0", "Hello World");
nextion_set_value("n0", 123);

// Veri alma (ayrı task'ta otomatik)
// nextion_data.action_type ve nextion_data.action_data kullanılır
```

## ⚙️ Konfigürasyon

### Menuconfig Ayarları:

```
Component config → uutils
├── Nextion Display Configuration
│   ├── Nextion TX Pin (43)
│   ├── Nextion RX Pin (44)
│   ├── Nextion Baud Rate (115200)
│   ├── Nextion Buffer Size (1024)
│   ├── Nextion Task Stack Size (4096)
│   ├── Nextion Task Priority (5)
│   ├── Nextion UART Port (2)
│   ├── Nextion Message Queue Size (20)
│   ├── Nextion Command Timeout (1000ms)
│   └── Enable Nextion Debug Logs (y)
└── NVS Configuration
    ├── NVS Namespace Size (32)
    ├── NVS Max String Length (256)
    ├── NVS Max Blob Size (1024)
    └── Enable NVS Debug Logs (y)
```

## 💻 Kullanım Örnekleri

### Tam Entegrasyon Örneği

```c
#include "u_math.h"
#include "u_nvs.h"
#include "u_nextion.h"
#include "esp_log.h"

static const char *TAG = "UUTILS_EXAMPLE";

void app_main(void) {
    ESP_LOGI(TAG, "UUtils kütüphanesi başlatılıyor...");
    
    // NVS başlatma
    esp_err_t ret = u_nvs_init("app_settings");
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "NVS başlatılamadı");
        return;
    }
    
    // Nextion başlatma
    nextion_init();
    
    // Ayarları yükle
    int32_t counter;
    if (u_nvs_get_int32("counter", &counter) != ESP_OK) {
        counter = 0; // Varsayılan değer
    }
    
    // Ekrana bilgi göster
    nextion_set_text("t0", "UUtils Test");
    nextion_set_value("n0", counter);
    
    // Ana döngü
    while (1) {
        // Sayaç artır
        counter++;
        u_nvs_set_int32("counter", counter);
        u_nvs_commit();
        
        // Ekranı güncelle
        nextion_set_value("n0", counter);
        
        // Basınç-hız hesaplama örneği
        float pressure = 25.0 + (counter % 50);
        float speed = pressure_to_speed(pressure);
        
        char speed_str[16];
        snprintf(speed_str, sizeof(speed_str), "%.1f", speed);
        nextion_set_text("t1", speed_str);
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

### Veri İşleme Örneği

```c
#include "u_math.h"

void process_sensor_data(void) {
    // Sensör verilerini sakla
    static float temperature_history[20];
    static uint16_t pressure_history[20];
    
    // Yeni veri ekle
    float new_temp = 25.5 + (rand() % 100) / 10.0;
    u_shift_and_insert_float(temperature_history, 20, new_temp);
    
    uint16_t new_pressure = 1000 + (rand() % 100);
    u_shift_and_insert_uint16(pressure_history, 20, new_pressure);
    
    // Ortalama hesapla
    float avg_temp = u_average_float(temperature_history, 20);
    uint16_t avg_pressure = u_average_uint16(pressure_history, 20);
    
    // Hız hesapla
    float speed = pressure_to_speed((float)avg_pressure);
    
    printf("Ortalama Sıcaklık: %.2f°C\n", avg_temp);
    printf("Ortalama Basınç: %d hPa\n", avg_pressure);
    printf("Hesaplanan Hız: %.2f m/s\n", speed);
}
```

## 🔧 API Referansı

### U_Math Fonksiyonları

```c
// Array işlemleri
void u_shift_and_insert_float(float arr[], int size, float new_value);
float u_average_float(float arr[], uint64_t count);
float u_sum_float(float arr[], uint64_t count);

// Dönüşüm fonksiyonları
float pressure_to_speed(float pressure);
uint8_t *u_hex_to_bytes(const char *hex, size_t *out_len);
void parse_string(const char *input, uint8_t *action_type, char *data_str);
```

### U_NVS Fonksiyonları

```c
// Başlatma ve sonlandırma
esp_err_t u_nvs_init(const char *namespace);
esp_err_t u_nvs_deinit(void);
bool u_nvs_is_initialized(void);

// Veri işlemleri
esp_err_t u_nvs_set_string(const char *key, const char *value);
esp_err_t u_nvs_get_string(const char *key, char *value, size_t max_length);
esp_err_t u_nvs_set_int32(const char *key, int32_t value);
esp_err_t u_nvs_get_int32(const char *key, int32_t *value);
esp_err_t u_nvs_set_uint32(const char *key, uint32_t value);
esp_err_t u_nvs_get_uint32(const char *key, uint32_t *value);
esp_err_t u_nvs_set_blob(const char *key, const void *value, size_t length);
esp_err_t u_nvs_get_blob(const char *key, void *value, size_t *length);

// Yardımcı fonksiyonlar
esp_err_t u_nvs_erase_all(void);
esp_err_t u_nvs_commit(void);
```

### U_Nextion Fonksiyonları

```c
// Başlatma ve kontrol
void nextion_init(void);
void nextion_listen_task(void *params);

// Komut gönderme
void nextion_send_command(const char *command);
void nextion_set_text(const char *component_id, const char *text);
void nextion_set_value(const char *component_id, int value);

// Veri yapısı
extern nextion_data_t nextion_data;
```

## ⚠️ Hata Yönetimi

### Yaygın Hata Kodları:

- `ESP_OK`: İşlem başarılı
- `ESP_ERR_INVALID_ARG`: Geçersiz parametre
- `ESP_ERR_INVALID_STATE`: Kütüphane başlatılmamış
- `ESP_ERR_INVALID_SIZE`: Buffer boyutu yetersiz
- `ESP_ERR_NVS_NOT_FOUND`: NVS anahtarı bulunamadı
- `ESP_ERR_TIMEOUT`: Timeout hatası

### Hata Kontrolü Örneği:

```c
esp_err_t ret = u_nvs_set_string("key", "value");
if (ret != ESP_OK) {
    ESP_LOGE(TAG, "NVS hatası: %s", esp_err_to_name(ret));
    
    switch (ret) {
        case ESP_ERR_INVALID_ARG:
            ESP_LOGE(TAG, "Geçersiz parametre");
            break;
        case ESP_ERR_INVALID_STATE:
            ESP_LOGE(TAG, "NVS başlatılmamış");
            break;
        default:
            ESP_LOGE(TAG, "Bilinmeyen hata");
            break;
    }
}
```

## 🚀 Performans

### Optimizasyonlar:

- **Bellek Verimliliği**: Dinamik bellek kullanımı minimize edilmiş
- **Thread Safety**: Semaphore ve queue kullanımı ile güvenli çoklu thread
- **Asenkron İşleme**: UART ve NVS işlemleri asenkron
- **Buffer Optimizasyonu**: Önceden ayrılmış buffer'lar ile düşük latency

### Performans Metrikleri:

- **NVS Yazma**: ~1-5ms (veri boyutuna bağlı)
- **NVS Okuma**: ~0.1-1ms
- **Array İşlemleri**: O(n) karmaşıklık
- **Nextion Komut**: ~1-10ms (komut boyutuna bağlı)

## 📝 Notlar

1. **Namespace Kullanımı**: Farklı uygulama bölümleri için farklı NVS namespace'leri kullanın
2. **Commit İşlemi**: NVS değişikliklerini commit etmeyi unutmayın
3. **Buffer Boyutları**: String ve blob boyutlarını aşmayın
4. **Thread Safety**: Çoklu thread kullanımında dikkatli olun
5. **Debug Logları**: Geliştirme sırasında açık, üretimde kapalı tutun

## 🤝 Katkıda Bulunma

Bu kütüphane ile ilgili sorularınız veya önerileriniz için:
- Issue açın
- Pull request gönderin
- Dokümantasyonu geliştirin

## 📄 Lisans

Bu kütüphane MIT lisansı altında lisanslanmıştır.

## 🔗 Bağlantılar

- [ESP-IDF Dokümantasyonu](https://docs.espressif.com/projects/esp-idf/)
- [Nextion Dokümantasyonu](https://nextion.tech/documents/)
- [NVS Kullanım Kılavuzu](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/storage/nvs_flash.html)
