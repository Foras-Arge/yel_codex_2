#include "u_nvs.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "U_NVS";

// Global variables
static nvs_handle_t nvs_handle_ptr = 0;
static char current_namespace[NVS_NAMESPACE_SIZE];
static bool nvs_initialized = false;

// Debug configuration
#ifdef CONFIG_NVS_DEBUG_ENABLE
#define NVS_DEBUG 1
#else
#define NVS_DEBUG 0
#endif

esp_err_t u_nvs_init(const char *namespace)
{
    if (nvs_initialized)
    {
        ESP_LOGW(TAG, "NVS already initialized");
        return ESP_OK;
    }

    if (namespace == NULL)
    {
        ESP_LOGE(TAG, "Namespace is NULL");
        return ESP_ERR_INVALID_ARG;
    }

    if (strlen(namespace) >= NVS_NAMESPACE_SIZE)
    {
        ESP_LOGE(TAG, "Namespace too long");
        return ESP_ERR_INVALID_ARG;
    }

    ESP_LOGI(TAG, "Initializing NVS with namespace: %s", namespace);

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // NVS partition was truncated and needs to be erased
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Open NVS handle
    ret = nvs_open(namespace, NVS_READWRITE, &nvs_handle_ptr);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to open NVS handle: %s", esp_err_to_name(ret));
        return ret;
    }

    // Store namespace name
    strncpy(current_namespace, namespace, sizeof(current_namespace) - 1);
    current_namespace[sizeof(current_namespace) - 1] = '\0';

    nvs_initialized = true;
    ESP_LOGI(TAG, "NVS initialized successfully");

    return ESP_OK;
}

esp_err_t u_nvs_deinit(void)
{
    if (!nvs_initialized)
    {
        ESP_LOGW(TAG, "NVS not initialized");
        return ESP_OK;
    }

    nvs_close(nvs_handle_ptr);

    nvs_handle_ptr = 0;
    nvs_initialized = false;
    ESP_LOGI(TAG, "NVS deinitialized successfully");

    return ESP_OK;
}

esp_err_t u_nvs_set_string(const char *key, const char *value)
{
    if (!nvs_initialized)
    {
        ESP_LOGE(TAG, "NVS not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    if (key == NULL || value == NULL)
    {
        ESP_LOGE(TAG, "Key or value is NULL");
        return ESP_ERR_INVALID_ARG;
    }

    if (strlen(value) >= NVS_MAX_STRING_LENGTH)
    {
        ESP_LOGE(TAG, "String value too long");
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t ret = nvs_set_str(nvs_handle_ptr, key, value);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to set string: %s", esp_err_to_name(ret));
        return ret;
    }

#if NVS_DEBUG
    ESP_LOGI(TAG, "Set string: %s = %s", key, value);
#endif

    return ESP_OK;
}

esp_err_t u_nvs_get_string(const char *key, char *value, size_t max_length)
{
    if (!nvs_initialized)
    {
        ESP_LOGE(TAG, "NVS not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    if (key == NULL || value == NULL)
    {
        ESP_LOGE(TAG, "Key or value pointer is NULL");
        return ESP_ERR_INVALID_ARG;
    }

    size_t required_size = 0;
    esp_err_t ret = nvs_get_str(nvs_handle_ptr, key, NULL, &required_size);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to get string size: %s", esp_err_to_name(ret));
        return ret;
    }

    if (required_size > max_length)
    {
        ESP_LOGE(TAG, "String too long for buffer");
        return ESP_ERR_INVALID_SIZE;
    }

    ret = nvs_get_str(nvs_handle_ptr, key, value, &required_size);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to get string: %s", esp_err_to_name(ret));
        return ret;
    }

#if NVS_DEBUG
    ESP_LOGI(TAG, "Get string: %s = %s", key, value);
#endif

    return ESP_OK;
}

esp_err_t u_nvs_delete_string(const char *key)
{
    if (!nvs_initialized)
    {
        ESP_LOGE(TAG, "NVS not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    if (key == NULL)
    {
        ESP_LOGE(TAG, "Key is NULL");
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t ret = nvs_erase_key(nvs_handle_ptr, key);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to delete string: %s", esp_err_to_name(ret));
        return ret;
    }

#if NVS_DEBUG
    ESP_LOGI(TAG, "Deleted string: %s", key);
#endif

    return ESP_OK;
}

esp_err_t u_nvs_set_int32(const char *key, int32_t value)
{
    if (!nvs_initialized)
    {
        ESP_LOGE(TAG, "NVS not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    if (key == NULL)
    {
        ESP_LOGE(TAG, "Key is NULL");
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t ret = nvs_set_i32(nvs_handle_ptr, key, value);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to set int32: %s", esp_err_to_name(ret));
        return ret;
    }

#if NVS_DEBUG
    ESP_LOGI(TAG, "Set int32: %s = %ld", key, (long)value);
#endif

    return ESP_OK;
}

esp_err_t u_nvs_get_int32(const char *key, int32_t *value)
{
    if (!nvs_initialized)
    {
        ESP_LOGE(TAG, "NVS not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    if (key == NULL || value == NULL)
    {
        ESP_LOGE(TAG, "Key or value pointer is NULL");
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t ret = nvs_get_i32(nvs_handle_ptr, key, value);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to get int32: %s", esp_err_to_name(ret));
        return ret;
    }

#if NVS_DEBUG
    ESP_LOGI(TAG, "Get int32: %s = %ld", key, (long)*value);
#endif

    return ESP_OK;
}

esp_err_t u_nvs_set_uint32(const char *key, uint32_t value)
{
    if (!nvs_initialized)
    {
        ESP_LOGE(TAG, "NVS not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    if (key == NULL)
    {
        ESP_LOGE(TAG, "Key is NULL");
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t ret = nvs_set_u32(nvs_handle_ptr, key, value);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to set uint32: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "Set uint32: %s = %lu", key, (unsigned long)value);

    return ESP_OK;
}

esp_err_t u_nvs_get_uint32(const char *key, uint32_t *value)
{
    if (!nvs_initialized)
    {
        ESP_LOGE(TAG, "NVS not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    if (key == NULL || value == NULL)
    {
        ESP_LOGE(TAG, "Key or value pointer is NULL");
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t ret = nvs_get_u32(nvs_handle_ptr, key, value);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to get uint32: %s", esp_err_to_name(ret));
        return ret;
    }

#if NVS_DEBUG
    ESP_LOGI(TAG, "Get uint32: %s = %lu", key, (unsigned long)*value);
#endif

    return ESP_OK;
}

esp_err_t u_nvs_set_blob(const char *key, const void *value, size_t length)
{
    if (!nvs_initialized)
    {
        ESP_LOGE(TAG, "NVS not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    if (key == NULL || value == NULL)
    {
        ESP_LOGE(TAG, "Key or value is NULL");
        return ESP_ERR_INVALID_ARG;
    }

    if (length > NVS_MAX_BLOB_SIZE)
    {
        ESP_LOGE(TAG, "Blob too large");
        return ESP_ERR_INVALID_SIZE;
    }

    esp_err_t ret = nvs_set_blob(nvs_handle_ptr, key, value, length);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to set blob: %s", esp_err_to_name(ret));
        return ret;
    }

#if NVS_DEBUG
    ESP_LOGI(TAG, "Set blob: %s (size: %zu)", key, length);
#endif

    return ESP_OK;
}

esp_err_t u_nvs_get_blob(const char *key, void *value, size_t *length)
{
    if (!nvs_initialized)
    {
        ESP_LOGE(TAG, "NVS not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    if (key == NULL || value == NULL || length == NULL)
    {
        ESP_LOGE(TAG, "Key, value, or length pointer is NULL");
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t ret = nvs_get_blob(nvs_handle_ptr, key, value, length);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to get blob: %s", esp_err_to_name(ret));
        return ret;
    }

#if NVS_DEBUG
    ESP_LOGI(TAG, "Get blob: %s (size: %zu)", key, *length);
#endif

    return ESP_OK;
}

esp_err_t u_nvs_delete_blob(const char *key)
{
    if (!nvs_initialized)
    {
        ESP_LOGE(TAG, "NVS not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    if (key == NULL)
    {
        ESP_LOGE(TAG, "Key is NULL");
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t ret = nvs_erase_key(nvs_handle_ptr, key);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to delete blob: %s", esp_err_to_name(ret));
        return ret;
    }

#if NVS_DEBUG
    ESP_LOGI(TAG, "Deleted blob: %s", key);
#endif

    return ESP_OK;
}

esp_err_t u_nvs_erase_all(void)
{
    if (!nvs_initialized)
    {
        ESP_LOGE(TAG, "NVS not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    esp_err_t ret = nvs_erase_all(nvs_handle_ptr);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to erase all: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "Erased all NVS data");
    return ESP_OK;
}

esp_err_t u_nvs_commit(void)
{
    if (!nvs_initialized)
    {
        ESP_LOGE(TAG, "NVS not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    esp_err_t ret = nvs_commit(nvs_handle_ptr);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to commit: %s", esp_err_to_name(ret));
        return ret;
    }

#if NVS_DEBUG
    ESP_LOGI(TAG, "NVS committed");
#endif

    return ESP_OK;
}

bool u_nvs_is_initialized(void)
{
    return nvs_initialized;
}