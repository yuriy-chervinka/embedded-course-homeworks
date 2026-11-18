idf_component_register(SRCS "module_2_5_prog_2.c" "encoder.c" "buzzer.c"
                        INCLUDE_DIRS "."
                        REQUIRES esp_driver_ledc esp_driver_gpio esp_timer)