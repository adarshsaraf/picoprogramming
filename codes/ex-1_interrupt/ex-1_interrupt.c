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

alarm_id_t my_alarm_id;

int64_t alarm_callback(alarm_id_t id, void *user_data) {
	printf("timer %d fired!\n", (int) id);
	gpio_put(LED_PIN, 1);
	if (gpio_get(BELT_PIN) == 0) {
		current_state = BELTED;
		gpio_put(LED_PIN, 0);
	}
	else if (gpio_get(SEAT_PIN) == 1) {
		gpio_put(LED_PIN, 0);
		current_state = IDLE;
	}
	else	
		current_state = BUZZER;
	//current_state = next_state;
	return 0;
}


void seat_on_callback(alarm_id_t id, void *user_data) {
	int seat = gpio_get(SEAT_PIN);
	int belt = gpio_get(BELT_PIN);
	if (!current_state) {
			gpio_put(LED_PIN, 0);
			if (seat == 0) {
				next_state = SEATED;
				add_alarm_in_ms(5000, alarm_callback, NULL, false);
			}
			else {
				next_state = IDLE;
				cancel_alarm(my_alarm_id);
			}
		}
		
		else if (current_state == SEATED) {
			if (belt == 0) {
				next_state = BELTED;
				cancel_alarm(my_alarm_id);
			}
			else {
				add_alarm_in_ms(5000, alarm_callback, NULL, false);
			}
		}
		
		else if ( current_state == BELTED ){
			if (belt == 1) {
				next_state = SEATED;
				add_alarm_in_ms(5000, alarm_callback, NULL, false);
			}
			gpio_put(LED_PIN, 0);
		}
		
		else if (current_state == BUZZER) {
			printf("BUZZED IN");
			gpio_put(LED_PIN, 1);
			if (seat == 1) {
				next_state = IDLE;
				cancel_alarm(my_alarm_id);
				gpio_put(LED_PIN, 0);
			}
			else if (belt == 0) {
				next_state = BELTED;
				gpio_put(LED_PIN, 0);
				cancel_alarm(my_alarm_id);
			}
			else	
				next_state = BUZZER;
		}
		printf("CURRENT STATE: %d\n", current_state);
		printf("NEXT STATE: %d\n", next_state);
		current_state = next_state;
}


int main(){
	stdio_init_all();
	
	//DECLARING ALL THE PINS
	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);
	
	gpio_init(BELT_PIN);
	gpio_set_dir(BELT_PIN, GPIO_IN);
	gpio_pull_up(BELT_PIN);
	
	gpio_init(SEAT_PIN);
	gpio_set_dir(SEAT_PIN, GPIO_IN);
	gpio_pull_up(SEAT_PIN);
	
	//Lower level interrupt function calls
	//gpio_set_irq_enabled(BELT_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, 1);
	//gpio_set_irq_enabled(SEAT_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, 1);
	//gpio_set_irq_callback(seat_on_callback);
	//gpio_add_raw_irq_handler_masked((0x1<<SEAT_PIN) | (0X1<<BELT_PIN), &seat_on_callback);
	//irq_set_enabled(IO_IRQ_BANK0, true);
	
	current_state = IDLE; //STATE IN IDLE WHEN NO INPUT
	cancel_alarm(my_alarm_id); 
	gpio_put(LED_PIN, 0);  // TURN OFF BUZZER IF ISR LEFT WHEN BUZZER ON
	
	//DECALRING PIN CHANGE INTERRUPT ROUTINE FOR SEAT PIN & BELT PIN
	gpio_set_irq_enabled_with_callback(SEAT_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, 1, &seat_on_callback);
	gpio_set_irq_enabled_with_callback(BELT_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, 1, &seat_on_callback);
	
	while(1){
		//current_state = IDLE; //STATE IN IDLE WHEN NO INPUT
		//cancel_alarm(my_alarm_id); 
		//gpio_put(LED_PIN, 0);  // TURN OFF BUZZER IF ISR LEFT WHEN BUZZER ON
		tight_loop_contents(); //KEEP TIGHT TIMING
		}
	
	return 0;
}
	
	
