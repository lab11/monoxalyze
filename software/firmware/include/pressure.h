#ifndef PRESSURE_H
#define PRESSURE_H

//configure pressure sensor with # defines


//initializes the pressure sensor to the above configuration
uint8_t pressureInit(void);

//sets the interrupt pressure threshold in Pa
void setPressureThreshold(uint32_t);

//sets the reference pressure in Pa
void setPressureReference(uint32_t);

//returns differential pressure in Pa
uint32_t getPressure(void);

//sets sample rate to that necessary to take a breath sample
//disables threshold interrupt
void setActive(void);

//sets sample rate to that necessary for Nordic to sleep and
//enables the threshold interrupt
void setInactive(void);

#endif
