#ifndef LMP91000_H
#define LMP91000_H

uint8_t gasInit();
uint32_t getGasSample();
uint32_t convertSampleToPPM();
uint8_t setGasActive();
uint8_t setGasInactive();

#endif
