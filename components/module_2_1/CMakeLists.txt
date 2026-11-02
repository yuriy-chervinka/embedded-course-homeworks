idf_component_register(SRCS "module_2_1.c" "led.c" "btn.c"
                       INCLUDE_DIRS "." "include"
                       REQUIRES esp_driver_gpio esp_timer)