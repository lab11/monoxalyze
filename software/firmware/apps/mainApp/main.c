
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "nordic_common.h"
#include "nrf.h"
#include "pins.h"
#include "led.h"
#include "bleApp.h"


int main(void)
{
    // Initialize
	appInit();
    // Enter main loop
    while(1)
    {
		//this runs the bleAPP state machine
		//that state machine generally looks like:
		//ADVERTISE
		//	IF CONNECT
		//		STREAM_DATA
		//		SLEEP
		//	ELSE
		//		SLEEP
		//
		//SLEEP
		//	IF TIMER_WAKEUP
		//		RESET_PRESSURE
		//		SLEEP
		//	ELIF PRESSURE_WAKEUP
		//		ADVERTISE
		//
		appService();
    }
}
