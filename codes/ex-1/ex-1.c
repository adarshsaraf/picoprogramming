#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"

#define LED_PIN 16
#define BELT_PIN 13
#define SEAT_PIN 12

alarm_id_t my_alarm_id;
int belted = 0;
int led_value = 0;


int64_t alarm_callback(alarm_id_t id, void *user_data) {
	printf("timer %d fired! LED on!\n", (int) id);
	led_value = 1;
	gpio_put(LED_PIN, 1);
	return 0;
}

/*
int64_t buzzer_off_callback(alarm_id_t id, void *user_data) {
	printf("timer %d fired!\n LED off!\n", (int) id);
	timer_fired = false;
	led_value = 0;
	gpio_put(LED_PIN, led_value);
	return 0;
}


void belt_on_callback(uint gpio, uint32_t events) {
	belted = 1;
	cancel_alarm(my_alarm_id);
	gpio_put(LED_PIN, 0);
	printf("Belt is in pin %d and is %d\n", gpio, events);
}

void belt_off_callback(uint gpio, uint32_t events) {
	//led_value = 0;
	//gpio_put(LED_PIN, led_value);
	add_alarm_in_ms(5000, alarm_callback, NULL, false);
	printf("belt is in pin %d and is %d\n", gpio, events);
}*/


int main(){
	stdio_init_all();
	
	uint state = 0;
	
	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);
	
	gpio_init(BELT_PIN);
	gpio_set_dir(BELT_PIN, GPIO_IN);
	gpio_pull_up(BELT_PIN);
	
	gpio_init(SEAT_PIN);
	gpio_set_dir(SEAT_PIN, GPIO_IN);
	gpio_pull_up(SEAT_PIN);
	
	while(1){
	state = state & 0;
	state = state | (gpio_get(SEAT_PIN) << 2) | (gpio_get(BELT_PIN) << 1) | led_value;
		switch(state) {
		case 0: case 1:
			printf("LED OFF\n");
			gpio_put(LED_PIN, 0);
			led_value = 0;
			break;
		case 4:
			add_alarm_in_ms(5000, alarm_callback, NULL, false);
			printf("alarm turned ON & %d\n!", state);
			break;
		case 7: case 6:
			cancel_alarm(my_alarm_id);
			printf("alarm cancelled! & %d & %d & %d\n", gpio_get(BELT_PIN), state, gpio_get(SEAT_PIN));
			gpio_put(LED_PIN, 0);
			led_value = 0;
			break;
		case 2: case 3:
			printf("NOT SEATED! BUT BELTED!\n");
			cancel_alarm(my_alarm_id);
			gpio_put(LED_PIN, 0);
			led_value = 0;
			break;
		}
		
	}
	
	//gpio_set_irq_enabled_with_callback(BELT_PIN, 0X4, 1, belt_on_callback);
	//gpio_set_irq_enabled_with_callback(BELT_PIN, 0X8, 1, belt_off_callback);
	
	//gpio_set_irq_enabled_with_callback(BELT_PIN, 0X4, 1, belt_on_callback);
	//gpio_set_irq_enabled_with_callback(BELT_PIN, 0X8, 1, cancel_alarm());
	
	return 0;
}
	
	
