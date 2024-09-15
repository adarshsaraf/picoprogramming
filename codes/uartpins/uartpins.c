#include<stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

int main(){
	
	stdio_init_all();
	
    uart_init(uart0, 115200);
    
    // Set the GPIO pin mux to the UART - 0 is TX, 1 is RX
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);

    uart_puts(uart0, "Welcome to the Jungle!");
    //uart_puts(uart0, "Whats up MF!\n");
	
}
