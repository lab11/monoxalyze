
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
	bleInit(sleep);
	//ledInit();
	//gasInit();
	//pressureInit();

    advertisingStart();

    // Enter main loop
    while(true);
    {
		bleService();
    }
}

void sleep(void) {

}
