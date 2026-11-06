idf_component_register(SRCS "module_1_proj.c" "adc.c" "filter.c"
                       INCLUDE_DIRS "." "include"
                       REQUIRES esp_adc esp_driver_gpio)