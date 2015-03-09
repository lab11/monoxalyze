
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "nordic_common.h"
#include "pins.h"
#include "nrf.h"
#include "led.h"
#include "lmp91000.h"
#include "lps25h.h"
#include "bleApp.h"

void sleep(void);

int main(void)
{
    // Initialize
	gasInit(); 
	pressureInit();
	bleInit(sleep);
    advertisingStart();
//	ledInit();
//	ledOn(LED_1);
	volatile int i = 0;	
    // Enter main loop
    while(true);
    {
	//	i++;
		bleService();
    }
}

void sleep(void) {

}
