# NVS (Non-Volatile Storage) Örnek Kullanımı

Bu dizin, ESP32/ESP8266 projelerinde NVS (Non-Volatile Storage) kullanımını gösteren örnek kodları içerir.

## 📋 İçindekiler

- [Genel Bakış](#genel-bakış)
- [Özellikler](#özellikler)
- [Kurulum](#kurulum)
- [Kullanım](#kullanım)
- [API Referansı](#api-referansı)
- [Örnek Kod](#örnek-kod)
- [Hata Yönetimi](#hata-yönetimi)
- [Konfigürasyon](#konfigürasyon)

## 🔍 Genel Bakış

NVS (Non-Volatile Storage), ESP32/ESP8266 cihazlarında kalıcı veri depolama için kullanılan bir sistemdir. Bu kütüphane, NVS işlemlerini basitleştiren wrapper fonksiyonları sağlar.

### NVS'nin Avantajları:
- **Kalıcı Depolama**: Cihaz yeniden başlatıldığında veriler korunur
- **Hızlı Erişim**: RAM'den daha yavaş ama flash'tan daha hızlı
- **Güvenilir**: Veri bütünlüğü korunur
- **Kolay Kullanım**: Basit API ile veri okuma/yazma

## ✨ Özellikler

### Desteklenen Veri Tipleri:
- **String**: Metin verileri
- **Int32**: 32-bit işaretli tam sayılar
- **Uint32**: 32-bit işaretsiz tam sayılar
- **Blob**: İkili veriler (struct, array vb.)

### Ana Fonksiyonlar:
- ✅ NVS başlatma ve sonlandırma
- ✅ Veri yazma (set) ve okuma (get)
- ✅ Veri silme
- ✅ Tüm verileri silme
- ✅ Değişiklikleri kaydetme (commit)
- ✅ Durum kontrolü

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

**Konfigürasyon Seçenekleri:**
- `CONFIG_NVS_NAMESPACE_SIZE`: Namespace boyutu (varsayılan: 16)
- `CONFIG_NVS_MAX_STRING_LENGTH`: Maksimum string uzunluğu (varsayılan: 256)
- `CONFIG_NVS_MAX_BLOB_SIZE`: Maksimum blob boyutu (varsayılan: 1024)
- `CONFIG_NVS_DEBUG_ENABLE`: Debug loglarını etkinleştir

## 📖 Kullanım

### Temel Kullanım Adımları:

1. **NVS'yi Başlatın**
```c
esp_err_t ret = u_nvs_init("my_namespace");
if (ret != ESP_OK) {
    // Hata işleme
}
```

2. **Veri Yazın**
```c
// String veri
u_nvs_set_string("device_name", "ESP32_Device");

// Sayısal veri
u_nvs_set_int32("counter", 123);
u_nvs_set_uint32("flags", 0xABCD);
```

3. **Veri Okuyun**
```c
char device_name[32];
u_nvs_get_string("device_name", device_name, sizeof(device_name));

int32_t counter;
u_nvs_get_int32("counter", &counter);
```

4. **Değişiklikleri Kaydedin**
```c
u_nvs_commit();
```

5. **Temizlik**
```c
u_nvs_deinit();
```

## 🔧 API Referansı

### Başlatma ve Sonlandırma

```c
esp_err_t u_nvs_init(const char *namespace);
esp_err_t u_nvs_deinit(void);
bool u_nvs_is_initialized(void);
```

### String İşlemleri

```c
esp_err_t u_nvs_set_string(const char *key, const char *value);
esp_err_t u_nvs_get_string(const char *key, char *value, size_t max_length);
esp_err_t u_nvs_delete_string(const char *key);
```

### Sayısal İşlemler

```c
esp_err_t u_nvs_set_int32(const char *key, int32_t value);
esp_err_t u_nvs_get_int32(const char *key, int32_t *value);
esp_err_t u_nvs_set_uint32(const char *key, uint32_t value);
esp_err_t u_nvs_get_uint32(const char *key, uint32_t *value);
```

### Blob İşlemleri

```c
esp_err_t u_nvs_set_blob(const char *key, const void *value, size_t length);
esp_err_t u_nvs_get_blob(const char *key, void *value, size_t *length);
esp_err_t u_nvs_delete_blob(const char *key);
```

### Yardımcı Fonksiyonlar

```c
esp_err_t u_nvs_erase_all(void);  // Tüm verileri sil
esp_err_t u_nvs_commit(void);     // Değişiklikleri kaydet
```

## 💻 Örnek Kod

### Basit Ayarlar Yönetimi

```c
#include "u_nvs.h"
#include "esp_log.h"

static const char *TAG = "SETTINGS";

// Ayarları kaydet
esp_err_t save_settings(void) {
    esp_err_t ret;
    
    // NVS'yi başlat
    ret = u_nvs_init("settings");
    if (ret != ESP_OK) return ret;
    
    // Ayarları kaydet
    ret = u_nvs_set_string("wifi_ssid", "MyWiFi");
    if (ret != ESP_OK) return ret;
    
    ret = u_nvs_set_string("wifi_pass", "password123");
    if (ret != ESP_OK) return ret;
    
    ret = u_nvs_set_int32("brightness", 80);
    if (ret != ESP_OK) return ret;
    
    ret = u_nvs_set_uint32("device_id", 0x12345678);
    if (ret != ESP_OK) return ret;
    
    // Değişiklikleri kaydet
    ret = u_nvs_commit();
    if (ret != ESP_OK) return ret;
    
    ESP_LOGI(TAG, "Settings saved successfully");
    return ESP_OK;
}

// Ayarları yükle
esp_err_t load_settings(void) {
    esp_err_t ret;
    char ssid[32], pass[64];
    int32_t brightness;
    uint32_t device_id;
    
    // NVS'yi başlat
    ret = u_nvs_init("settings");
    if (ret != ESP_OK) return ret;
    
    // Ayarları oku
    ret = u_nvs_get_string("wifi_ssid", ssid, sizeof(ssid));
    if (ret != ESP_OK) return ret;
    
    ret = u_nvs_get_string("wifi_pass", pass, sizeof(pass));
    if (ret != ESP_OK) return ret;
    
    ret = u_nvs_get_int32("brightness", &brightness);
    if (ret != ESP_OK) return ret;
    
    ret = u_nvs_get_uint32("device_id", &device_id);
    if (ret != ESP_OK) return ret;
    
    ESP_LOGI(TAG, "SSID: %s, Pass: %s, Brightness: %ld, Device ID: 0x%lX",
             ssid, pass, (long)brightness, (unsigned long)device_id);
    
    return ESP_OK;
}
```

### Struct Veri Saklama

```c
typedef struct {
    char name[32];
    uint32_t id;
    int16_t temperature;
    bool enabled;
} device_config_t;

esp_err_t save_device_config(const device_config_t *config) {
    return u_nvs_set_blob("device_config", config, sizeof(device_config_t));
}

esp_err_t load_device_config(device_config_t *config) {
    size_t size = sizeof(device_config_t);
    return u_nvs_get_blob("device_config", config, &size);
}
```

## ⚠️ Hata Yönetimi

### Yaygın Hata Kodları:

- `ESP_ERR_INVALID_ARG`: Geçersiz parametre
- `ESP_ERR_INVALID_STATE`: NVS başlatılmamış
- `ESP_ERR_INVALID_SIZE`: Buffer boyutu yetersiz
- `ESP_ERR_NVS_NOT_FOUND`: Anahtar bulunamadı
- `ESP_ERR_NVS_NO_FREE_PAGES`: NVS alanı dolu

### Hata Kontrolü Örneği:

```c
esp_err_t ret = u_nvs_set_string("key", "value");
if (ret != ESP_OK) {
    ESP_LOGE(TAG, "NVS error: %s", esp_err_to_name(ret));
    
    switch (ret) {
        case ESP_ERR_INVALID_ARG:
            ESP_LOGE(TAG, "Invalid argument provided");
            break;
        case ESP_ERR_INVALID_STATE:
            ESP_LOGE(TAG, "NVS not initialized");
            break;
        case ESP_ERR_NVS_NO_FREE_PAGES:
            ESP_LOGE(TAG, "NVS storage full");
            break;
        default:
            ESP_LOGE(TAG, "Unknown error");
            break;
    }
}
```

## ⚙️ Konfigürasyon

### Menuconfig Ayarları:

```
Component config → uutils → NVS Configuration
├── NVS namespace size (16)
├── NVS max string length (256)
├── NVS max blob size (1024)
└── Enable NVS debug logs (n)
```

### Önerilen Ayarlar:

- **Namespace Size**: 16-32 karakter (çok uzun olmasın)
- **Max String Length**: 256-512 byte (ihtiyaca göre)
- **Max Blob Size**: 1024-4096 byte (struct boyutuna göre)
- **Debug Logs**: Geliştirme sırasında açık, üretimde kapalı

## 🔍 Debug ve Test

### Debug Loglarını Etkinleştirme:

```c
// menuconfig'de CONFIG_NVS_DEBUG_ENABLE'i açın
// veya kodda debug logları ekleyin
#ifdef CONFIG_NVS_DEBUG_ENABLE
    ESP_LOGI(TAG, "Setting key: %s = %s", key, value);
#endif
```

### NVS İçeriğini Kontrol Etme:

```c
// Tüm anahtarları listele (ESP-IDF'nin nvs_dump tool'u ile)
idf.py monitor
# Monitor'da: nvs_dump
```

## 📝 Notlar

1. **Namespace Kullanımı**: Farklı uygulama bölümleri için farklı namespace'ler kullanın
2. **Commit İşlemi**: Değişiklikler commit edilmeden kalıcı olmaz
3. **Boyut Sınırları**: String ve blob boyutlarını aşmayın
4. **Hata Kontrolü**: Her NVS işleminden sonra hata kontrolü yapın
5. **Performans**: Çok sık yazma işlemlerinden kaçının

## 🤝 Katkıda Bulunma

Bu kütüphane ile ilgili sorularınız veya önerileriniz için:
- Issue açın
- Pull request gönderin
- Dokümantasyonu geliştirin

## 📄 Lisans

Bu kütüphane MIT lisansı altında lisanslanmıştır. 