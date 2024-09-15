#include <stdio.h>
#include "pico/stlib.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"

bool repeating_timer_callback(alarm_id_t id, void *user_data){
	printf("Timer %d fired!\n", (int) id);
	timer_fired = true;
	return 0;
}

 
