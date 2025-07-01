idf_component_register(
    SRCS "honeywell.c"
    INCLUDE_DIRS "include" 
    REQUIRES esp_timer log driver freertos uutils
)