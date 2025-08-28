#include "_wifi.h"
#include "esp_log.h"

static const char *TAG = "STATIC_IP_EXAMPLE";

void static_ip_example_usage(void)
{
    // Debug modunu etkinleştir
    _wifi_set_debug(true);

    // WiFi'yi başlat
    _wifi_init();

    // Statik IP ayarlarını değiştirme örnekleri:

    // 1. Farklı bir statik IP ayarla
    uint8_t new_ip[4] = {192, 168, 2, 150};
    uint8_t new_gateway[4] = {192, 168, 2, 1};
    uint8_t new_netmask[4] = {255, 255, 255, 0};

    // _wifi_set_static_ip(true, new_ip, new_gateway, new_netmask);

    // 2. DHCP'ye geri dön
    // _wifi_set_static_ip(false, NULL, NULL, NULL);

    // 3. Farklı ağ için statik IP
    uint8_t office_ip[4] = {10, 0, 0, 100};
    uint8_t office_gateway[4] = {10, 0, 0, 1};
    uint8_t office_netmask[4] = {255, 255, 255, 0};

    // _wifi_set_static_ip(true, office_ip, office_gateway, office_netmask);

    ESP_LOGI(TAG, "Statik IP örnek kullanımı başlatıldı");
}

// Statik IP ayarlarını kontrol etme örneği
void check_static_ip_settings(void)
{
    if (g_wifi_config.static_ip.use_static_ip)
    {
        ESP_LOGI(TAG, "Statik IP aktif:");
        ESP_LOGI(TAG, "IP: %d.%d.%d.%d",
                 g_wifi_config.static_ip.ip[0], g_wifi_config.static_ip.ip[1],
                 g_wifi_config.static_ip.ip[2], g_wifi_config.static_ip.ip[3]);
        ESP_LOGI(TAG, "Gateway: %d.%d.%d.%d",
                 g_wifi_config.static_ip.gateway[0], g_wifi_config.static_ip.gateway[1],
                 g_wifi_config.static_ip.gateway[2], g_wifi_config.static_ip.gateway[3]);
        ESP_LOGI(TAG, "Netmask: %d.%d.%d.%d",
                 g_wifi_config.static_ip.netmask[0], g_wifi_config.static_ip.netmask[1],
                 g_wifi_config.static_ip.netmask[2], g_wifi_config.static_ip.netmask[3]);
    }
    else
    {
        ESP_LOGI(TAG, "DHCP kullanılıyor");
    }
}