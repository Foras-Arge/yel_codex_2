idf_component_register(
    SRCS "screen_fw.c"
    INCLUDE_DIRS "include" 
    REQUIRES esp_timer log driver freertos sd_card uutils
)