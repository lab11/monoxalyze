
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "nordic_common.h"
#include "nrf.h"
//#include "pins.h"
//#include "led.h"
//#include "bleApp.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"


int main(void)
{
    // Initialize
	appInit();

	//NRF_POWER->TASKS_LOWPWR = 1;
	__WFI();

   	/*while(1)
    {     
		if(nrf_gpio_pin_read(17) == 0)
        {
            // Clear LED0
            //nrf_gpio_pin_clear(LED0);

            // Enter system OFF. After wakeup the chip will be reset, and the MCU will run from the top 
        }
    }*/
}

