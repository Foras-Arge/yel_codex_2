idf_component_register(
    SRCS "u_nextion.c" "u_nvs.c" "u_math.c"
    INCLUDE_DIRS "include"
    REQUIRES driver esp_timer nvs_flash
)