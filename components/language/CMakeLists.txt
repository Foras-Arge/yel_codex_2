idf_component_register(
    SRCS "language.c"
    INCLUDE_DIRS "include" 
    REQUIRES esp_timer log  uutils
)