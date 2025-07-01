#pragma once

#include <stdio.h>
#include <stdint.h>
#include "esp_err.h"
#include "nvs_flash.h"

// NVS Configuration from menuconfig
#define NVS_NAMESPACE_SIZE 32
#define NVS_MAX_STRING_LENGTH 256
#define NVS_MAX_BLOB_SIZE 1024

// Function prototypes
esp_err_t u_nvs_init(const char *namespace);
esp_err_t u_nvs_deinit(void);

// String operations
esp_err_t u_nvs_set_string(const char *key, const char *value);
esp_err_t u_nvs_get_string(const char *key, char *value, size_t max_length);
esp_err_t u_nvs_delete_string(const char *key);

// Integer operations
esp_err_t u_nvs_set_int32(const char *key, int32_t value);
esp_err_t u_nvs_get_int32(const char *key, int32_t *value);
esp_err_t u_nvs_set_uint32(const char *key, uint32_t value);
esp_err_t u_nvs_get_uint32(const char *key, uint32_t *value);

// Blob operations
esp_err_t u_nvs_set_blob(const char *key, const void *value, size_t length);
esp_err_t u_nvs_get_blob(const char *key, void *value, size_t *length);
esp_err_t u_nvs_delete_blob(const char *key);

// Utility functions
esp_err_t u_nvs_erase_all(void);
esp_err_t u_nvs_commit(void);
bool u_nvs_is_initialized(void);