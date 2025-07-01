idf_component_register(
    SRCS "_rtc.c" "i2c_bus_soft.c"
    INCLUDE_DIRS "include" 
    PRIV_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../espressif__i2c_bus/include"
    REQUIRES esp_timer log driver espressif__i2c_bus freertos
)