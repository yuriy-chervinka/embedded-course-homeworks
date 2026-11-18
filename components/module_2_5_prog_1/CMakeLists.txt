idf_component_register(SRCS "module_2_5_prog_1.c" "timer.c"
                        INCLUDE_DIRS "."
                        REQUIRES esp_driver_gptimer esp_driver_gpio)