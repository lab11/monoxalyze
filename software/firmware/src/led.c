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

    NRF_RNG->TASKS_START = 1;
    NRF_RNG->EVENTS_VALRDY = 0;
    while(NRF_RNG->EVENTS_VALRDY == 0);
    srand(NRF_RNG->VALUE);
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

//turns the leds into a random configuration and returns that
//configuration in octal
uint8_t genRandomLed(void) {
    uint8_t r = rand() % 8; 
    nrf_gpio_pin_write(LED_1, r & 0x01);
    nrf_gpio_pin_write(LED_2, r & 0x02);
    nrf_gpio_pin_write(LED_3, r & 0x04);
    return r;
}
