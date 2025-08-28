#include "sd_card.h"
#include "esp_vfs_fat.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

static const char *TAG = "SD_LIB";
static sdmmc_card_t *mounted_card = NULL;
static bool is_sd_mounted = false;
static bool spi_initialized = false;
static sd_card_status_t current_sd_status = SD_CARD_REMOVED;
static sd_card_status_callback_t status_callback = NULL;
static TaskHandle_t sd_detect_task_handle = NULL;
int mean_count = 0;

esp_err_t init_spi_bus(void)
{
    if (!spi_initialized)
    {
        spi_bus_config_t bus_config = {
            .mosi_io_num = VSPI_PIN_NUM_MOSI,
            .miso_io_num = VSPI_PIN_NUM_MISO,
            .sclk_io_num = VSPI_PIN_NUM_CLK,
            .quadwp_io_num = -1,
            .quadhd_io_num = -1,
            .max_transfer_sz = 4000};

        esp_err_t ret = spi_bus_initialize(SPI3_HOST, &bus_config, SPI_DMA_CH_AUTO);
        if (ret != ESP_OK)
        {
#if SD_DEBUG
            ESP_LOGE("SPI_BUS", "SPI bus initialization failed: %s", esp_err_to_name(ret));
#endif
            return ret;
        }
#if SD_DEBUG
        ESP_LOGI("SPI_BUS", "SPI initialized successfully");
#endif
        spi_initialized = true;
    }
    return ESP_OK;
}

esp_err_t deinit_spi_bus(void)
{
    if (spi_initialized)
    {
        esp_err_t ret = spi_bus_free(SPI3_HOST);
        if (ret != ESP_OK)
        {
#if SD_DEBUG
            ESP_LOGE("SPI_BUS", "SPI kapatılırken bir hata oluştu");
#endif
            return ret;
        }
#if SD_DEBUG
        ESP_LOGI("SPI_BUS", "SPI başarıyla kapatıldı.");
#endif
        spi_initialized = false;
    }
    return ESP_OK;
}

esp_err_t mount_sd_card(void)
{
    const int max_retries = 3;
    int retry_count = 0;

    while (retry_count < max_retries)
    {
        if (is_sd_mounted)
        {
            return ESP_OK;
        }

        // Initialize SPI bus
        esp_err_t ret = init_spi_bus();
        if (ret != ESP_OK)
        {
#if SD_DEBUG
            ESP_LOGE(TAG, "Failed to initialize SPI bus");
#endif
            return ret;
        }

        // Configure SD card
        sdmmc_host_t host = SDSPI_HOST_DEFAULT();
        host.slot = SPI3_HOST;

        sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
        slot_config.gpio_cs = VSPI_PIN_NUM_CS;
        slot_config.host_id = host.slot;

        esp_vfs_fat_sdmmc_mount_config_t mount_config = {
            .format_if_mount_failed = false,
            .max_files = 5,
            .allocation_unit_size = 16 * 1024};

        // Try to mount
        ret = esp_vfs_fat_sdspi_mount("/sdcard", &host, &slot_config, &mount_config, &mounted_card);

        if (ret == ESP_OK)
        {
#if SD_DEBUG
            ESP_LOGI(TAG, "SD card mounted successfully");
#endif
            is_sd_mounted = true;
            return ESP_OK;
        }
#if SD_DEBUG
        ESP_LOGW(TAG, "Failed to mount SD card (attempt %d/%d): %s",
                 retry_count + 1, max_retries, esp_err_to_name(ret));
#endif

        // Cleanup before retry
        deinit_spi_bus();
        vTaskDelay(pdMS_TO_TICKS(1000));
        retry_count++;
    }
#if SD_DEBUG
    ESP_LOGE(TAG, "Failed to mount SD card after %d attempts", max_retries);
#endif
    return ESP_FAIL;
}

void unmount_sd_card(void)
{
    if (is_sd_mounted)
    {
        esp_vfs_fat_sdcard_unmount("/sdcard", mounted_card);
#if SD_DEBUG
        ESP_LOGI(TAG, "SD kart unmount edildi.");
#endif
        mounted_card = NULL;
        is_sd_mounted = false;
    }
    else
    {
#if SD_DEBUG
        ESP_LOGW(TAG, "SD kart zaten unmount edilmiş.");
#endif
    }
}

esp_err_t sd_init(void)
{
    // Configure detect pin
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << VSPI_PIN_NUM_DETECT),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE};
    gpio_config(&io_conf);

    // Check if card is present
    if (gpio_get_level(VSPI_PIN_NUM_DETECT) != 0)
    {
        current_sd_status = SD_CARD_REMOVED;
        if (status_callback)
        {
            status_callback(SD_CARD_REMOVED, "SD kart bulunamadı");
        }
#if SD_DEBUG
        ESP_LOGE(TAG, "No SD card detected");
#endif
        return ESP_ERR_NOT_FOUND;
    }

    gpio_set_pull_mode(VSPI_PIN_NUM_MISO, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(VSPI_PIN_NUM_MOSI, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(VSPI_PIN_NUM_CLK, GPIO_PULLUP_ONLY);

    // Try to mount the card
    esp_err_t ret = mount_sd_card();
    if (ret != ESP_OK)
    {
        current_sd_status = SD_CARD_ERROR;
        if (status_callback)
        {
            status_callback(SD_CARD_ERROR, "SD kart başlatılamadı");
        }
#if SD_DEBUG
        ESP_LOGE(TAG, "Failed to initialize SD card: %s", esp_err_to_name(ret));
#endif
        return ret;
    }

    // SD kart algılama görevini başlat (eğer zaten çalışmıyorsa)
    if (sd_detect_task_handle == NULL)
    {
        BaseType_t task_created = xTaskCreatePinnedToCore(sd_detect_task, "sd_detect_task", 1024 * 4, NULL, 5, &sd_detect_task_handle, 1);
        if (task_created != pdPASS)
        {
            ESP_LOGE(TAG, "SD algılama görevi oluşturulamadı");
            return ESP_FAIL;
        }
    }

    current_sd_status = SD_CARD_INSERTED;
    if (status_callback)
    {
        status_callback(SD_CARD_INSERTED, "SD kart başarıyla başlatıldı");
    }

    return ESP_OK;
}

esp_err_t sd_write(const char *filename, const char *data)
{
    if (!is_sd_mounted)
    {
#if SD_DEBUG
        ESP_LOGE(TAG, "SD kart monte edilmemiş. Yazma işlemi başarısız.");
#endif
        return ESP_ERR_INVALID_STATE;
    }
    char file_name[256] = {0};
    sprintf(file_name, "/sdcard/%s", filename);
    FILE *f = fopen(file_name, "w");
    if (f == NULL)
    {
#if SD_DEBUG
        ESP_LOGE(TAG, "Dosya açılamadı: %s", file_name);
#endif
        return ESP_FAIL;
    }

    size_t written = fwrite(data, 1, strlen(data), f);
    fclose(f);

    if (written != strlen(data))
    {
#if SD_DEBUG
        ESP_LOGE(TAG, "Tüm veriler yazılamadı. Yazılan: %zu, Beklenen: %zu", written, strlen(data));
#endif
        return ESP_FAIL;
    }
#if SD_DEBUG
    ESP_LOGI(TAG, "Dosya başarıyla yazıldı: %s", file_name);
#endif
    return ESP_OK;
}

esp_err_t sd_read(const char *filename, char *buffer, size_t buffer_size)
{
    if (!is_sd_mounted)
    {
#if SD_DEBUG
        ESP_LOGE(TAG, "SD kart monte edilmemiş. Okuma işlemi başarısız.");
#endif
        return ESP_ERR_INVALID_STATE;
    }
    char file_name[512] = {0};
    sprintf(file_name, "/sdcard/%s", filename);
    FILE *f = fopen(file_name, "r");
    if (f == NULL)
    {
#if SD_DEBUG
        ESP_LOGE(TAG, "Dosya açılamadı: %s", file_name);
#endif
        return ESP_FAIL;
    }

    size_t read = fread(buffer, 1, buffer_size - 1, f);
    fclose(f);

    if (read == 0)
    {
#if SD_DEBUG
        ESP_LOGE(TAG, "Dosya okunamadı: %s", file_name);
#endif
        return ESP_FAIL;
    }

    buffer[read] = '\0'; // Sonlandırma karakteri ekle
#if SD_DEBUG
    ESP_LOGI(TAG, "Dosya başarıyla okundu: %s", file_name);
#endif
    return ESP_OK;
}

esp_err_t sd_append(const char *filename, const char *data)
{
    if (!is_sd_mounted)
    {
#if SD_DEBUG
        ESP_LOGE(TAG, "SD kart monte edilmemiş. Yazma işlemi başarısız.");
#endif
        return ESP_ERR_INVALID_STATE;
    }
    char file_name[256] = {0};
    sprintf(file_name, "/sdcard/%s", filename);
    FILE *f = fopen(file_name, "a"); // Dosya yoksa oluştur
    if (f == NULL)
    {
#if SD_DEBUG
        ESP_LOGE(TAG, "Dosya açılamadı: %s, errno: %d", file_name, errno);
#endif
        return ESP_FAIL;
    }

    size_t written = fprintf(f, "%s\n", data); // Alt satıra ekleme
    fclose(f);

    if (written < strlen(data))
    {
#if SD_DEBUG
        ESP_LOGE(TAG, "Tüm veriler yazılamadı. Yazılan: %zu, Beklenen: %zu", written, strlen(data));
#endif
        return ESP_FAIL;
    }

#if SD_DEBUG
    ESP_LOGI(TAG, "Dosyaya başarıyla alt satıra yazıldı: %s", file_name);
#endif
    return ESP_OK;
}

esp_err_t delete_file_from_sd_card(const char *filename)
{
    if (!is_sd_mounted)
    {
#if SD_DEBUG
        ESP_LOGE(TAG, "SD kart monte edilmemiş. Dosya silme işlemi başarısız.");
#endif
        return ESP_ERR_INVALID_STATE;
    }
    char file_name[256] = {0};
    sprintf(file_name, "/sdcard/%s", filename);
    int ret = remove(file_name);
    if (ret != 0)
    {
#if SD_DEBUG
        ESP_LOGE(TAG, "Dosya silinemedi: %s", file_name);
#endif
        return ESP_FAIL;
    }
#if SD_DEBUG
    ESP_LOGI(TAG, "Dosya başarıyla silindi: %s", file_name);
#endif
    return ESP_OK;
}

void delete_folder_recursive(const char *folder_path)
{
    DIR *dir = opendir(folder_path);
    if (dir == NULL)
    {
#if SD_DEBUG
        ESP_LOGE(TAG, "Klasör açılamadı: %s, errno: %d", folder_path, errno);
#endif
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        // '.' ve '..' dizinlerini atla
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        // Alt elemanın tam yolunu oluştur
        char path[256]; // Reduced from 512
        int ret = snprintf(path, sizeof(path), "%s/%s", folder_path, entry->d_name);
        if (ret < 0 || ret >= sizeof(path))
        {
#if SD_DEBUG
            ESP_LOGE(TAG, "Yol oluşturulamadı, tampon taşması: %s/%s", folder_path, entry->d_name);
#endif
            continue;
        }

        struct stat st;
        if (stat(path, &st) == 0)
        {
            if (S_ISDIR(st.st_mode))
            {
                // Alt klasörü rekursif olarak sil
                delete_folder_recursive(path);
            }
            else
            {
                // Dosyayı sil
                if (remove(path) == 0)
                {
#if SD_DEBUG
                    ESP_LOGI(TAG, "Dosya silindi: %s", path);
#endif
                }
                else
                {
#if SD_DEBUG
                    ESP_LOGE(TAG, "Dosya silinemedi: %s, errno: %d", path, errno);
#endif
                }
            }
        }
    }

    closedir(dir);

    // Klasörü sil
    if (rmdir(folder_path) == 0)
    {
        ESP_LOGI(TAG, "Klasör silindi: %s", folder_path);
    }
    else
    {
        ESP_LOGE(TAG, "Klasör silinemedi: %s, errno: %d", folder_path, errno);
    }
}

int create_directory(const char *path)
{
    struct stat st = {0};
    // Klasörün mevcut olup olmadığını kontrol et
    char path_copy[256] = {0};
    sprintf(path_copy, "/sdcard/%s", path);
    if (stat(path_copy, &st) == -1)
    {
        // Klasörü oluştur
        if (mkdir(path_copy, 0777) == 0)
        {
#if SD_DEBUG
            ESP_LOGI(TAG, "Klasör başarıyla oluşturuldu: %s", path_copy);
#endif
            return 1;
        }
        else
        {
#if SD_DEBUG
            ESP_LOGE(TAG, "Klasör oluşturulamadı: %s, errno: %d", path_copy, errno);
#endif
            return 0;
        }
    }
    else
    {
#if SD_DEBUG
        ESP_LOGI(TAG, "Klasör zaten mevcut: %s", path_copy);
#endif
        return 2;
    }
}

void sd_save_data(const char *data_type, const char *uid, const char *data, const char *time)
{
    char sd_data_path[50] = {0};
    char sd_data[50] = {0};
    sprintf(sd_data_path, "/sdcard/Devices/%s", uid);
    create_directory(sd_data_path);

    sprintf(sd_data_path, "/sdcard/Devices/%s/%s.txt", uid, data_type);

    if (access(sd_data_path, F_OK) == 0)
    {
#if SD_DEBUG
        ESP_LOGI("SD_CARD", "%s dosyası bulundu.", sd_data_path);
#endif
        sprintf(sd_data, "%s,%s", data, time);
        sd_append(sd_data_path, sd_data);
    }
    else
    {
#if SD_DEBUG
        ESP_LOGI("SD_CARD", "%s dosyası bulunamadı.", sd_data_path);
#endif
        sprintf(sd_data, "%s,%s\n", data, time);
        sd_write(sd_data_path, sd_data);
    }
}

esp_err_t read_last_line_from_file(const char *file_path, char *last_line, size_t max_length)
{
    char file_path_full[512] = {0};
    snprintf(file_path_full, sizeof(file_path_full), "/sdcard/%s", file_path);

    FILE *file = fopen(file_path_full, "r");
    if (file == NULL)
    {
#if SD_DEBUG
        ESP_LOGE(TAG, "Dosya açılamadı: %s, errno: %d", file_path, errno);
#endif
        return ESP_FAIL;
    }

    char buffer[2048]; // Tek satır için buffer
    last_line[0] = '\0';

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        // Satır sonundaki \n veya \r\n karakterlerini kaldır
        size_t len = strlen(buffer);
        while (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r'))
        {
            buffer[--len] = '\0';
        }

        // Son satırı kopyala
        strncpy(last_line, buffer, max_length - 1);
        last_line[max_length - 1] = '\0'; // Güvenli null-terminator
    }

    fclose(file);

    if (strlen(last_line) == 0)
    {
#if SD_DEBUG
        ESP_LOGW(TAG, "Dosya boş veya son satır okunamadı: %s", file_path);
#endif
        return ESP_FAIL;
    }

#if SD_DEBUG
    ESP_LOGI(TAG, "Son satır (temizlenmiş): %s", last_line);
#endif
    return ESP_OK;
}

esp_err_t read_last_n_lines(const char *file_path, int n, char lines[][256], int *line_count)
{
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
    {
#if SD_DEBUG
        ESP_LOGE(TAG, "Dosya açılamadı: %s, errno: %d", file_path, errno);
#endif
        return ESP_FAIL;
    }

    char buffer[2048];       // Satır başına maksimum 2048 karakter
    char temp_lines[n][128]; // Son N satırı geçici olarak saklamak için tampon
    int current_index = 0;   // Döngü için kullanılan indeks
    int total_lines = 0;     // Toplam satır sayısı

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        // Geçici tamponda dairesel bir kaydırma yap
        strncpy(temp_lines[current_index], buffer, sizeof(temp_lines[current_index]) - 1);
        temp_lines[current_index][sizeof(temp_lines[current_index]) - 1] = '\0'; // Null terminator ekle

        current_index = (current_index + 1) % n; // Dairesel indeks
        total_lines++;
    }

    fclose(file);

    // Okunan satırları sıralı olarak `lines` tamponuna kopyala
    int start_index = (total_lines < n) ? 0 : current_index;
    int count = (total_lines < n) ? total_lines : n;

    for (int i = 0; i < count; i++)
    {
        strncpy(lines[i], temp_lines[(start_index + i) % n], sizeof(lines[i]) - 1);
        lines[i][sizeof(lines[i]) - 1] = '\0'; // Null terminator ekle
    }

    *line_count = count; // Toplam okunan satır sayısını döndür

    if (count == 0)
    {
#if SD_DEBUG
        ESP_LOGW(TAG, "Dosya boş veya satır okunamadı: %s", file_path);
#endif
        return ESP_FAIL;
    }

#if SD_DEBUG
    ESP_LOGI(TAG, "Son %d satır başarıyla okundu.", count);
#endif
    mean_count = count;
    return ESP_OK;
}

esp_err_t perform_ota_from_sd(void)
{
#if SD_DEBUG
    ESP_LOGI("OTA", "OTA işlemi SD karttan başlatılıyor: %s", "/sdcard/Update/firmware.bin");
#endif

    FILE *f = fopen("/sdcard/Update/firmware.bin", "rb");
    if (f == NULL)
    {
#if SD_DEBUG
        ESP_LOGE("OTA", "Firmware dosyası açılamadı. SD kartın doğru monte edildiğinden emin olun.");
#endif
        return ESP_FAIL;
    }

    const esp_partition_t *ota_partition = esp_ota_get_next_update_partition(NULL);
    if (ota_partition == NULL)
    {
#if SD_DEBUG
        ESP_LOGE("OTA", "OTA partition bulunamadı.");
#endif
        fclose(f);
        return ESP_FAIL;
    }

    esp_ota_handle_t ota_handle;
    esp_err_t ret = esp_ota_begin(ota_partition, OTA_SIZE_UNKNOWN, &ota_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE("OTA", "OTA başlatılamadı. Hata: %s", esp_err_to_name(ret));
        fclose(f);
        return ret;
    }

    char buffer[2048];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), f)) > 0)
    {
        ret = esp_ota_write(ota_handle, buffer, bytes_read);
        if (ret != ESP_OK)
        {
            ESP_LOGE("OTA", "OTA yazma hatası: %s", esp_err_to_name(ret));
            fclose(f);
            esp_ota_end(ota_handle);
            return ret;
        }
    }

    if (ferror(f))
    {
        ESP_LOGE("OTA", "Dosya okuma hatası.");
        fclose(f);
        esp_ota_end(ota_handle);
        return ESP_FAIL;
    }

    fclose(f);

    ret = esp_ota_end(ota_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE("OTA", "OTA sonlandırılamadı. Hata: %s", esp_err_to_name(ret));
        return ret;
    }

    ret = esp_ota_set_boot_partition(ota_partition);
    if (ret != ESP_OK)
    {
        ESP_LOGE("OTA", "Yeni firmware set edilemedi. Hata: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI("OTA", "OTA işlemi başarıyla tamamlandı. Sistemi yeniden başlatıyorum...");
    delete_file_from_sd_card("/sdcard/Update/firmware.bin");
    return ESP_OK;
}

esp_err_t check_and_perform_ota(void)
{
    esp_err_t ret = ESP_FAIL;

    if (is_sd_mounted && access("/sdcard/Update/firmware.bin", F_OK) == 0)
    {
#if SD_DEBUG
        ESP_LOGI("SD_CARD", "OTA dosyası bulundu.");
#endif
        ret = ESP_OK;
    }
    else
    {
#if SD_DEBUG
        ESP_LOGI("SD_CARD", "OTA dosyası bulunamadı.");
#endif
    }
    return ret;
}

void sd_detect_task(void *params)
{
    ESP_LOGI(TAG, "SD kart algılama görevi başlatıldı");

    // İlk durumu kontrol et
    bool card_present = (gpio_get_level(VSPI_PIN_NUM_DETECT) == 0);

    if (card_present)
    {
        ESP_LOGI(TAG, "SD kart tespit edildi, başlatılıyor...");
        if (sd_init() == ESP_OK)
        {
            current_sd_status = SD_CARD_INSERTED;
            if (status_callback)
            {
                status_callback(SD_CARD_INSERTED, "SD kart başarıyla takıldı ve hazır");
            }
            ESP_LOGI(TAG, "SD kart başarıyla başlatıldı");
        }
        else
        {
            current_sd_status = SD_CARD_ERROR;
            if (status_callback)
            {
                status_callback(SD_CARD_ERROR, "SD kart başlatılamadı");
            }
            ESP_LOGE(TAG, "SD kart başlatılamadı");
        }
    }
    else
    {
        current_sd_status = SD_CARD_REMOVED;
        if (status_callback)
        {
            status_callback(SD_CARD_REMOVED, "SD kart bulunamadı");
        }
        ESP_LOGW(TAG, "SD kart bulunamadı, takılması bekleniyor...");

        // SD kart takılmasını bekle
        while (gpio_get_level(VSPI_PIN_NUM_DETECT) == 1)
        {
            vTaskDelay(pdMS_TO_TICKS(100));
        }

        // SD kart takıldı, başlat
        ESP_LOGI(TAG, "SD kart takıldı, başlatılıyor...");
        if (sd_init() == ESP_OK)
        {
            current_sd_status = SD_CARD_INSERTED;
            if (status_callback)
            {
                status_callback(SD_CARD_INSERTED, "SD kart başarıyla takıldı ve hazır");
            }
            ESP_LOGI(TAG, "SD kart başarıyla başlatıldı");
        }
        else
        {
            current_sd_status = SD_CARD_ERROR;
            if (status_callback)
            {
                status_callback(SD_CARD_ERROR, "SD kart başlatılamadı");
            }
            ESP_LOGE(TAG, "SD kart başlatılamadı");
        }
    }

    // Sürekli durum kontrolü
    bool previous_card_present = card_present;

    while (1)
    {
        card_present = (gpio_get_level(VSPI_PIN_NUM_DETECT) == 0);

        if (card_present != previous_card_present)
        {
            if (card_present)
            {
                // SD kart takıldı
                ESP_LOGI(TAG, "SD kart takıldı, başlatılıyor...");
                vTaskDelay(pdMS_TO_TICKS(500)); // Kartın stabil olması için bekle

                if (sd_init() == ESP_OK)
                {
                    current_sd_status = SD_CARD_INSERTED;
                    if (status_callback)
                    {
                        status_callback(SD_CARD_INSERTED, "SD kart başarıyla takıldı ve hazır");
                    }
                    ESP_LOGI(TAG, "SD kart başarıyla başlatıldı");
                }
                else
                {
                    current_sd_status = SD_CARD_ERROR;
                    if (status_callback)
                    {
                        status_callback(SD_CARD_ERROR, "SD kart başlatılamadı, kart arızalı olabilir");
                    }
                    ESP_LOGE(TAG, "SD kart başlatılamadı, kart arızalı olabilir");
                }
            }
            else
            {
                // SD kart çıkarıldı
                ESP_LOGW(TAG, "SD kart çıkarıldı!");
                if (status_callback)
                {
                    status_callback(SD_CARD_REMOVED, "SD kart çıkarıldı");
                }

                // Güvenli şekilde unmount et
                vTaskDelay(pdMS_TO_TICKS(100));
                unmount_sd_card();
                vTaskDelay(pdMS_TO_TICKS(100));
                deinit_spi_bus();

                current_sd_status = SD_CARD_REMOVED;
                ESP_LOGI(TAG, "SD kart güvenli şekilde çıkarıldı");
            }
            previous_card_present = card_present;
        }

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

bool file_exists(const char *path)
{
    struct stat st;
    return (stat(path, &st) == 0);
}

// Yeni eklenen fonksiyonlar

void sd_set_status_callback(sd_card_status_callback_t callback)
{
    status_callback = callback;
}

sd_card_status_t sd_get_card_status(void)
{
    return current_sd_status;
}

bool sd_is_card_mounted(void)
{
    return is_sd_mounted;
}

esp_err_t sd_force_reinit(void)
{
    ESP_LOGI(TAG, "SD kart yeniden başlatılıyor...");

    // Mevcut durumu temizle
    if (is_sd_mounted)
    {
        unmount_sd_card();
    }
    if (spi_initialized)
    {
        deinit_spi_bus();
    }

    // Yeni başlatma dene
    esp_err_t ret = sd_init();
    if (ret == ESP_OK)
    {
        current_sd_status = SD_CARD_INSERTED;
        if (status_callback)
        {
            status_callback(SD_CARD_INSERTED, "SD kart başarıyla yeniden başlatıldı");
        }
    }
    else
    {
        current_sd_status = SD_CARD_ERROR;
        if (status_callback)
        {
            status_callback(SD_CARD_ERROR, "SD kart yeniden başlatılamadı");
        }
    }

    return ret;
}

esp_err_t sd_init_with_callback(sd_card_status_callback_t callback)
{
    status_callback = callback;
    return sd_init();
}
