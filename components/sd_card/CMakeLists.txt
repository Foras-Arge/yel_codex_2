idf_component_register(
    SRCS "sd_card.c"
    INCLUDE_DIRS "include" 
    REQUIRES esp_timer log driver freertos fatfs app_update esp_http_client)