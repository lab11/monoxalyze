#ifndef BATTERY_H
#define BATTERY_H

//sets up interrupts for battery charging pin
uint8_t batteryInit(void);
uint8_t getBatteryPercentage(void);
uint8_t isCharging(void);
uint8_t isCharged(void);
uint8_t isLow(void);

#endif
