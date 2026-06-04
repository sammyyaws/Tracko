#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include <freertos/task.h>
#include <driver/gpio.h>


void app_main(void)
{
gpio_num_t pin=GPIO_NUM_2;
gpio_set_direction(pin,GPIO_MODE_OUTPUT);

while(true){
    gpio_set_level(pin,1);
    vTaskDelay(200/portTICK_PERIOD_MS);
    gpio_set_level(pin,0);
    vTaskDelay(200/portTICK_PERIOD_MS);
}


}