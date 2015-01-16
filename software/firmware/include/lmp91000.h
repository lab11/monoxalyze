#ifndef LMP91000_H
#define LMP91000_H

uint8_t gasInit();
uint16_t getPPM();
uint16_t getTemp();
void setGasActive();
void setGasInactive();

#endif
