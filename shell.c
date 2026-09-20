#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pico/stdlib.h>
#include "pico/cyw43_arch.h"

const uint LED_PIN = CYW43_WL_GPIO_LED_PIN; // Define the LED pin for the Raspberry Pi Pico

int main(void){ 
    stdio_init_all(); // Initialize standard I/O for the shell

    if (cyw43_arch_init()) { // Initialize the Wi-Fi/LED driver
        printf("Wi-Fi/LED driver initialization failed!\n");
        return -1;
    }

    while (!stdio_usb_connected()) { // Wait for the USB connection to be established
        sleep_ms(100);
    }
    
    sleep_ms(2000); // Wait for 2 seconds to allow the user to open a terminal and connect to the UART shell simulator

    int rx_byte; // Variable to store the received byte
    char buffer[64]; // Buffer to store the received bytes
    size_t buffer_index = 0; // Index to keep track of the buffer position

    printf("Shell Initialized. Type a command and press Enter.\n> ");
    fflush(stdout); // Flush the output buffer to ensure the prompt is displayed immediately

    while(true){
        
        rx_byte = getchar_timeout_us(0); // Perform a non-blocking check for incoming USB serial data; returns PICO_ERROR_TIMEOUT if buffer is empty
        
        // If no character is ready, skip processing and loop again
        if (rx_byte == PICO_ERROR_TIMEOUT) {
            tight_loop_contents(); // Good practice to give the CPU a tiny yield hint
            continue;
        }

        if(rx_byte == EOF){ // Handle end of input (Ctrl+D)
            printf("\nEnd of input detected. Exiting.\n");
            break;
        }

        if (rx_byte == '\b' || rx_byte == 127) { // Handle backspace (ASCII 8 or 127)
            if (buffer_index > 0) {
                buffer_index--;
                printf("\b \b"); 
                fflush(stdout);
            }
            continue;
        }

        if(rx_byte == '\n' || rx_byte == '\r'){
            buffer[buffer_index] = '\0'; // Null terminate the string
            printf("\r\n"); // Move cursor to a new line before executing

            //Check what the user typed
            if (strcmp(buffer, "help") == 0) {
                printf("Available commands:\r\n");
                printf("  help    - Show this menu\r\n");
                printf("  led_on  - Turn the green onboard LED ON\r\n");
                printf("  led_off - Turn the green onboard LED OFF\r\n");
            } 
            else if (strcmp(buffer, "led_on") == 0) {
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
                printf("LED status: ON\r\n");
            } 
            else if (strcmp(buffer, "led_off") == 0) {
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
                printf("LED status: OFF\r\n");
            } 
            else if (strlen(buffer) > 0) { // If they typed something but it didn't match any known command
                printf("Unknown command: '%s'. Type 'help' for options.\r\n", buffer);
            }

            buffer_index = 0; // Reset buffer for the next input
            printf("> "); 
            fflush(stdout);
        } else {
            if(buffer_index < sizeof(buffer) - 1){  // Ensure space for null terminator
                buffer[buffer_index++] = (char)rx_byte; 
                putchar(rx_byte); 
                fflush(stdout);
            } else { // Buffer overflow protection
                printf("\r\nBuffer overflow. Command too long.\r\n"); 
                buffer_index = 0;
                printf("> "); 
                fflush(stdout);
            }
        }
    }
    return 0;
}
