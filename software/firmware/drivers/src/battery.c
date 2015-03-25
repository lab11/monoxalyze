
#include <stdint.h>
#include <stdbool.h>
#include "adc.h"
#include "nrf.h"
#include "nrf_gpio.h"
#include "pins.h"
#include "battery.h"

//sets up interrupts for battery charging pin

static volatile int lastBatteryPercentage;
static volatile int currentBatteryPercentage;
static bool charging = 0;
static bool charged = 0;

uint8_t batteryInit(void) {
	//config CHARGE pin as gpio input
	nrf_gpio_cfg_input(CHRG,NRF_GPIO_PIN_NOPULL);
	getBatteryPercentage();
    return 0;
}

uint8_t getBatteryPercentage(void) {

	adcConfig(ADC_CONFIG_RES_10bit, ADC_CONFIG_INPSEL_AnalogInputNoPrescaling,  
	        	ADC_CONFIG_REFSEL_VBG,                      
				1 << BATIN,                                  
				ADC_CONFIG_EXTREFSEL_None);     

	lastBatteryPercentage = currentBatteryPercentage;
	currentBatteryPercentage = getSample();

	if(currentBatteryPercentage > lastBatteryPercentage) {
		charging = 1;
	} else if (currentBatteryPercentage == lastBatteryPercentage) {

	} else {
		charging = 0;
	}

	return currentBatteryPercentage;
}

uint8_t isCharging(void) {	
	
	getBatteryPercentage();

	return charging;
}

uint8_t isCharged(void) {	
	//sample charge pin
	charged = nrf_gpio_pin_read(CHRG);

	if(charged) {
		charging = 0;
	}

	return  charged;
}

uint8_t isLow(void) {
	if(getBatteryPercentage() < 172) {
		return 1;
	} else {
		return 0;
	}
}
