// sd.h
#ifndef SD_H
#define SD_H

#include "sdmmc_cmd.h"
#include <stdbool.h>

#define VSPI_PIN_NUM_MISO 13
#define VSPI_PIN_NUM_MOSI 11
#define VSPI_PIN_NUM_CLK 12
#define VSPI_PIN_NUM_CS 10
#define VSPI_PIN_NUM_DETECT 9

// SD kart durumu için enum
typedef enum
{
    SD_CARD_REMOVED = 0,
    SD_CARD_INSERTED = 1,
    SD_CARD_ERROR = 2
} sd_card_status_t;

// SD kart durum değişikliği callback fonksiyonu tipi
typedef void (*sd_card_status_callback_t)(sd_card_status_t status, const char *message);

extern int mean_count;

// Temel SD kart fonksiyonları
esp_err_t sd_init();
esp_err_t sd_write(const char *filename, const char *data);
esp_err_t sd_append(const char *filename, const char *data);
esp_err_t sd_read(const char *filename, char *buffer, size_t buffer_size);
esp_err_t perform_ota_from_sd(void);
esp_err_t check_and_perform_ota(void);
void unmount_sd_card(void);
esp_err_t deinit_spi_bus(void);
void sd_save_data(const char *data_type, const char *uid, const char *data, const char *time);
int create_directory(const char *path);
void delete_folder_recursive(const char *folder_path);
esp_err_t read_last_line_from_file(const char *file_path, char *last_line, size_t max_length);
esp_err_t read_last_n_lines(const char *file_path, int n, char lines[][256], int *line_count);
void sd_detect_task(void *params);
bool file_exists(const char *path);

// Yeni eklenen fonksiyonlar
esp_err_t sd_init_with_callback(sd_card_status_callback_t callback);
sd_card_status_t sd_get_card_status(void);
bool sd_is_card_mounted(void);
esp_err_t sd_force_reinit(void);
void sd_set_status_callback(sd_card_status_callback_t callback);

#endif // SD_H
