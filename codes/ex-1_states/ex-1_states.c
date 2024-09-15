#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"

#define LED_PIN 16
#define BELT_PIN 13
#define SEAT_PIN 12

#define IDLE 0
#define SEATED 1
#define BELTED 2
#define BUZZER 3

int current_state = IDLE;
int next_state = IDLE;


int64_t alarm_callback(alarm_id_t id, void *user_data) {
	printf("timer %d fired! LED on!\n", (int) id);
	//led_value = 1;
	if (gpio_get(BELT_PIN) == 0)
		next_state = BELTED;
	else	
		next_state = BUZZER;
	gpio_put(LED_PIN, 0);
	current_state = next_state;
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
	
		if (!current_state) {
			if (gpio_get(SEAT_PIN) == 0) 
				next_state = SEATED;
			else
				next_state = IDLE;
			
			gpio_put(LED_PIN, 0);
		}
		
		else if (current_state == SEATED) {
			if (gpio_get(BELT_PIN) == 0)
				next_state = BELTED;
			else {
				add_alarm_in_ms(5000, alarm_callback, NULL, false);
				//sleep_ms(5000);
			}
		}
		
		else if ( current_state == BELTED ){
			if (gpio_get(BELT_PIN) == 1)
				next_state = SEATED;
			gpio_put(LED_PIN, 0);
		}
		
		else if (current_state == BUZZER) {
			gpio_put(LED_PIN, 1);
			if (gpio_get(SEAT_PIN) == 1) 
				next_state = IDLE;
			else if (gpio_get(BELT_PIN) == 0) {
				next_state = BELTED;
				gpio_put(LED_PIN, 0);
			}
			else	
				next_state = BUZZER;
		}
		printf("CURRENT STATE: %d\n", current_state);
		printf("NEXT STATE: %d\n", next_state);
		current_state = next_state;
	}
	
	//gpio_set_irq_enabled_with_callback(BELT_PIN, 0X4, 1, belt_on_callback);
	//gpio_set_irq_enabled_with_callback(BELT_PIN, 0X8, 1, belt_off_callback);
	
	//gpio_set_irq_enabled_with_callback(BELT_PIN, 0X4, 1, belt_on_callback);
	//gpio_set_irq_enabled_with_callback(BELT_PIN, 0X8, 1, cancel_alarm());
	
	return 0;
}
	
	
