idf_component_register(
    SRCS "motor_driver.c"
    INCLUDE_DIRS "include" 
    REQUIRES esp_timer log driver freertos
)