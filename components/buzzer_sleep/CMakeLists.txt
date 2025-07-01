idf_component_register(
    SRCS "buzzer_sleep.c"
    INCLUDE_DIRS "include" 
    REQUIRES esp_timer log driver freertos esp_adc
)