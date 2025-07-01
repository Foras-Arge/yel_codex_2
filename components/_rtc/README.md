# PCF8563T RTC Komponenti

Bu komponent, PCF8563T gerçek zamanlı saat (RTC) modülü için ESP-IDF tabanlı bir sürücü sağlar. PCF8563T, düşük güç tüketimi ve yüksek hassasiyetli zaman tutma özellikleri ile bilinen popüler bir RTC modülüdür.

## Özellikler

- ✅ PCF8563T RTC modülü ile tam uyumluluk
- ✅ Yazılım I2C desteği (herhangi bir GPIO pinini kullanabilir)
- ✅ Donanım I2C desteği
- ✅ BCD formatında zaman okuma/yazma
- ✅ String formatında zaman ayarlama
- ✅ Epoch zaman dönüşümü
- ✅ Türkiye saat dilimi (UTC+3) desteği
- ✅ Kapsamlı hata kontrolü
- ✅ Debug log desteği

## Donanım Gereksinimleri

### PCF8563T RTC Modülü Bağlantıları

| PCF8563T Pin | ESP32 Pin | Açıklama |
|--------------|-----------|----------|
| VCC          | 3.3V      | Güç kaynağı |
| GND          | GND       | Toprak |
| SDA          | GPIO 18   | I2C Data (varsayılan) |
| SCL          | GPIO 17   | I2C Clock (varsayılan) |

### Pin Konfigürasyonu

Pin numaralarını `idf.py menuconfig` komutu ile değiştirebilirsiniz:
- **RTC Options** → **PCF8563T RTC Options** → **PCF8563T SDA Pin**
- **RTC Options** → **PCF8563T RTC Options** → **PCF8563T SCL Pin**

## Kurulum

### 1. Komponenti Projenize Ekleyin

```bash
# Proje dizininizde components klasörü oluşturun (yoksa)
mkdir -p components

# _rtc komponentini kopyalayın
cp -r _rtc components/
```

### 2. CMakeLists.txt'ye Ekleyin

Ana `CMakeLists.txt` dosyanızda:

```cmake
set(EXTRA_COMPONENT_DIRS "components")
```

### 3. Konfigürasyon

```bash
idf.py menuconfig
```

**RTC Options** menüsünden:
- SDA ve SCL pin numaralarını ayarlayın
- I2C frekansını belirleyin (varsayılan: 100kHz)
- Yazılım I2C'yi etkinleştirin
- Debug loglarını etkinleştirin (geliştirme için)

## Kullanım

### Temel Kullanım

```c
#include "_rtc.h"

void app_main(void)
{
    // RTC'yi başlat
    esp_err_t ret = _rtc_init();
    if (ret != ESP_OK) {
        ESP_LOGE("MAIN", "RTC başlatılamadı!");
        return;
    }

    // Saat ayarla (saat:dakika formatında)
    pcf8563t_set_time_from_string("14,30");  // 14:30

    // Tarih ayarla (gün,ay,yıl formatında)
    pcf8563t_set_date_from_string("15,12,2024");  // 15 Aralık 2024

    // Zamanı oku ve formatla
    pcf8563t_formatted_time_t formatted_time;
    pcf8563t_get_formatted_time(&formatted_time, true);
    printf("Şu anki zaman: %s\n", formatted_time.formatted_time);
}
```

### Gelişmiş Kullanım

```c
#include "_rtc.h"

void rtc_example(void)
{
    // RTC'yi başlat
    _rtc_init();

    // Manuel zaman ayarlama
    pcf8563t_time_t time = {
        .seconds = 0,
        .minutes = 30,
        .hours = 14,
        .day = 15,
        .weekday = 1,  // Pazartesi
        .month = 12,
        .year = 24     // 2024
    };
    pcf8563t_set_time(&time);

    // Zamanı oku
    pcf8563t_time_t current_time;
    pcf8563t_get_time(&current_time);
    
    printf("Saat: %02d:%02d:%02d\n", 
           current_time.hours, 
           current_time.minutes, 
           current_time.seconds);
    printf("Tarih: %02d/%02d/20%02d\n", 
           current_time.day, 
           current_time.month, 
           current_time.year);

    // Epoch zamanını al
    time_t epoch = pcf8563t_get_epoch_time();
    printf("Epoch time: %ld\n", epoch);

    // Ayrı ayrı string değerleri al
    char day_str[4], month_str[4], year_str[6];
    pcf8563t_get_day_string(day_str, sizeof(day_str));
    pcf8563t_get_month_string(month_str, sizeof(month_str));
    pcf8563t_get_year_string(year_str, sizeof(year_str));
    
    printf("Gün: %s, Ay: %s, Yıl: %s\n", day_str, month_str, year_str);
}
```

## API Referansı

### Başlatma ve Temizleme

```c
esp_err_t _rtc_init(void);
void pcf8563t_deinit(void);
```

### Temel Zaman İşlemleri

```c
pcf8563t_err_t pcf8563t_set_time(pcf8563t_time_t *time);
pcf8563t_err_t pcf8563t_get_time(pcf8563t_time_t *time);
```

### String Formatında Zaman İşlemleri

```c
pcf8563t_err_t pcf8563t_set_time_from_string(const char *time_str);  // "hh,mm"
pcf8563t_err_t pcf8563t_set_date_from_string(const char *date_str);  // "dd,mm,yyyy"
```

### Formatlanmış Zaman

```c
pcf8563t_err_t pcf8563t_get_formatted_time(pcf8563t_formatted_time_t *formatted_time, bool is_display);
```

### Ayrı Değerler

```c
pcf8563t_err_t pcf8563t_get_day_string(char *day_str, size_t size);
pcf8563t_err_t pcf8563t_get_month_string(char *month_str, size_t size);
pcf8563t_err_t pcf8563t_get_year_string(char *year_str, size_t size);
```

### Epoch Zamanı

```c
time_t pcf8563t_get_epoch_time(void);
```

## Veri Yapıları

### Zaman Yapısı

```c
typedef struct {
    uint8_t seconds;    // 0-59
    uint8_t minutes;    // 0-59
    uint8_t hours;      // 0-23
    uint8_t day;        // 1-31
    uint8_t weekday;    // 0-6 (Pazar=0)
    uint8_t month;      // 1-12
    uint16_t year;      // 0-99 (2000-2099)
} pcf8563t_time_t;
```

### Formatlanmış Zaman Yapısı

```c
typedef struct {
    char hour[4];           // "14"
    char minute[4];         // "30"
    char day[4];            // "15"
    char month[4];          // "12"
    char year[6];           // "2024"
    char formatted_time[50]; // "14:30 15.12.2024"
} pcf8563t_formatted_time_t;
```

### Hata Kodları

```c
typedef enum {
    PCF8563T_OK = 0,
    PCF8563T_ERROR_INIT = -1,
    PCF8563T_ERROR_WRITE = -2,
    PCF8563T_ERROR_READ = -3
} pcf8563t_err_t;
```

## Konfigürasyon Seçenekleri

| Seçenek | Varsayılan | Açıklama |
|---------|------------|----------|
| `PCF8563T_SDA_PIN` | 18 | SDA GPIO pin numarası |
| `PCF8563T_SCL_PIN` | 17 | SCL GPIO pin numarası |
| `PCF8563T_I2C_FREQ` | 100000 | I2C frekansı (Hz) |
| `PCF8563T_ENABLE_SOFTWARE_I2C` | y | Yazılım I2C kullanımı |
| `PCF8563T_TIMEZONE_OFFSET` | 3 | UTC'den saat farkı |
| `PCF8563T_ENABLE_DEBUG` | n | Debug logları |

## PCF8563T Register Haritası

| Register | Adres | Açıklama |
|----------|-------|----------|
| Control1 | 0x00 | Kontrol register 1 |
| Control2 | 0x01 | Kontrol register 2 |
| VL_Seconds | 0x02 | Saniye (VL bit dahil) |
| Minutes | 0x03 | Dakika |
| Hours | 0x04 | Saat |
| Days | 0x05 | Gün |
| Weekdays | 0x06 | Haftanın günü |
| Century_Months | 0x07 | Ay |
| Years | 0x08 | Yıl |

## Hata Ayıklama

Debug loglarını etkinleştirmek için:

```bash
idf.py menuconfig
# RTC Options → PCF8563T RTC Options → Enable PCF8563T Debug Logging
```

Debug logları şunları içerir:
- I2C başlatma hataları
- Zaman okuma/yazma hataları
- Geçersiz format hataları

## Sorun Giderme

### RTC Bağlantı Sorunları

1. **Pin bağlantılarını kontrol edin**
   - SDA ve SCL pinlerinin doğru bağlandığından emin olun
   - Pull-up dirençlerinin (4.7kΩ) bağlı olduğunu kontrol edin

2. **I2C frekansını düşürün**
   - Uzun kablolar için 50kHz kullanın
   - `PCF8563T_I2C_FREQ` değerini düşürün

3. **Debug loglarını kontrol edin**
   - `PCF8563T_ENABLE_DEBUG` seçeneğini etkinleştirin
   - Serial monitörde hata mesajlarını inceleyin

### Zaman Tutma Sorunları

1. **Pil bağlantısını kontrol edin**
   - PCF8563T'nin CR2032 pilinin takılı olduğundan emin olun
   - Pil voltajının 2.5V üzerinde olduğunu kontrol edin

2. **VL bitini kontrol edin**
   - VL (Voltage Low) biti set edilmişse, pil zayıf demektir
   - Yeni pil takın ve zamanı yeniden ayarlayın

## Örnek Projeler

### Basit Saat Uygulaması

```c
#include "_rtc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void clock_task(void *pvParameters)
{
    _rtc_init();
    
    while (1) {
        pcf8563t_formatted_time_t time;
        pcf8563t_get_formatted_time(&time, true);
        printf("Saat: %s\n", time.formatted_time);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    xTaskCreate(clock_task, "clock", 2048, NULL, 5, NULL);
}
```

### Alarm Uygulaması

```c
#include "_rtc.h"

void check_alarm(void)
{
    pcf8563t_time_t current_time;
    pcf8563t_get_time(&current_time);
    
    // 08:00'da alarm
    if (current_time.hours == 8 && current_time.minutes == 0 && current_time.seconds == 0) {
        printf("ALARM! Saat 08:00\n");
        // Alarm işlemleri burada
    }
}
```


