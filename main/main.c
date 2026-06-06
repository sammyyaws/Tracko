#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "sim800.h"
static const char *TAG = "GPS_NEO6M";

#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_16)
#define UART_PORT_NUM (UART_NUM_2)

#define RX_BUF_SIZE (1024)

void init_uart(void) {
    const uart_config_t uart_config = {
        .baud_rate = 9600,             // NEO-6M default baud rate is 9600
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    // Install UART driver, and filter out invert flags
    ESP_ERROR_CHECK(uart_driver_install(UART_PORT_NUM, RX_BUF_SIZE * 2, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UART_PORT_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_PORT_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
}

// Simple helper function to print out NMEA strings cleanly
void parse_nmea_stream(uint8_t *data, int len) {
    char *line = strtok((char *)data, "\r\n");
    while (line != NULL) {
        // Look for standard GPS sentences
        if (strncmp(line, "$GPRMC", 6) == 0 || strncmp(line, "$GPGGA", 6) == 0) {
            ESP_LOGI(TAG, "Received GPS Line: %s", line);
            
            // TODO: Pass 'line' to an NMEA parser library (like MinMEA) 
            // to extract float coordinates for Latitude and Longitude.
        }
        line = strtok(NULL, "\r\n");
    }
}

void gps_rx_task(void *arg) {
    uint8_t *data = (uint8_t *) malloc(RX_BUF_SIZE + 1);
    
    while (1) {
        // Read data from the UART
        int len = uart_read_bytes(UART_PORT_NUM, data, RX_BUF_SIZE, 20 / portTICK_PERIOD_MS);
        
        if (len > 0) {
            data[len] = '\0'; // Null-terminate the string buffer
            parse_nmea_stream(data, len);
        }
    }
    free(data);
}

void app_main(void) {
    // Initialize the configured UART port
    init_uart();
    init_sim_uart();
    ESP_LOGI(TAG, "UART initialized. Waiting for NEO-6M data...");

    // Create a background task to handle incoming serial data
    xTaskCreate(gps_rx_task, "gps_rx_task", 4096, NULL, 5, NULL);
}