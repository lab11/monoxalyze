#ifndef LED_H
#define LED_H

uint8_t ledInit();
void ledOn(uint8_t);
void ledOff(uint8_t);
void ledToggle(uint8_t);

//turns the leds into a random configuration and returns that
//configuration in octal
uint8_t genRandomLed(void);

#endif
