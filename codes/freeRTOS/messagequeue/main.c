#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "message_buffer.h"

const size_t BUFFER_SIZE = 32;

void vReceiverTask(void *pvParameters) {

	MessageBufferHandle_t buffer = (MessageBufferHandle_t)pvParameters;
	
	size_t messageSize = BUFFER_SIZE - 4;
	char *message = malloc(messageSize);
	memset(message, '\0', messageSize);
	size_t lengthReceived;
	
	for(;;) {
		lengthReceived = xMessageBufferReceive(buffer, (void *)message, BUFFER_SIZE, 0);
		
		if (lengthRe
