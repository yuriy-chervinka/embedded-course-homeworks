idf_component_register(SRCS "module_2_2_prog_2.c" "peripherals.c"
                       INCLUDE_DIRS "." "include"
                       REQUIRES esp_driver_gpio esp_timer)