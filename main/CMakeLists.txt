idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES _rtc uutils honeywell motor_driver sd_card buzzer_sleep log screen_fw wifi language)