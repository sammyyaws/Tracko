#include "sim800.h"

#include "stdio.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "string.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include  "esp_log.h"

#define  TX_PIN (GPIO_NUM_17)
#define  RX_PIN (GPIO_NUM_16)
#define  UART_PIN_CONF (GPIO_NUM_2)
#define RX_BUF_SIZE (1024)


void init_sim_uart(void){
    const  uart_config_t uart_config={
    .baud_rate = 9600,             
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,


    };
    ESP_ERROR_CHECK(uart_driver_install(UART_PIN_CONF,RX_BUF_SIZE *2,0,0,NULL,0));
    ESP_ERROR_CHECK(uart_param_config())
    
}

