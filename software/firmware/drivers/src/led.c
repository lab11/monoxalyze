#include <stdint.h>
#include <stdlib.h>
#include "led.h"
#include "pins.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"

uint8_t ledInit() {
    nrf_gpio_cfg_output(LED_1);
    nrf_gpio_cfg_output(LED_2);
    nrf_gpio_cfg_output(LED_3);

    return 0;
}

void ledOn(uint8_t led) {
    nrf_gpio_pin_set(led);
}

void ledOff(uint8_t led) {
    nrf_gpio_pin_clear(led);
}

void ledToggle(uint8_t led) {
    nrf_gpio_pin_toggle(led);
}

void ledSet(uint8_t led, uint8_t on) {
	if(on) {
    	nrf_gpio_pin_set(led);
	} else {
		nrf_gpio_pin_clear(led);
	}
}
