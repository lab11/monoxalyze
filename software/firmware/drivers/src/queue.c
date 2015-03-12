#include "queue.h"

#define QUEUE_SIZE 40

static int queue[QUEUE_SIZE] = {0}; 
static int pushPointer = 0;

void queuePush(int add) {
	queue[pushPointer] = add;
	pushPointer++;
	if(pushPointer >= QUEUE_SIZE) {
		pushPointer = 0;
	}
}

int queueAverage(void) {
	int i = 0;
	int sum = 0;
	for(i = 0; i < QUEUE_SIZE; i++) {
		sum += queue[i];
	}

	return sum/QUEUE_SIZE;
}
