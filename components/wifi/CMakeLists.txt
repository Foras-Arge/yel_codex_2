idf_component_register(
    SRCS  "_mqtt.c" "_wifi.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_wifi esp_event esp_netif mqtt
)