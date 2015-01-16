
#include <stdint.h>
#include "pressure.h"
#include "twi_master.h"

//initializes the pressure sensor to the above configuration
uint8_t pressureInit() {
    twi_master_init();
}

//sets the interrupt pressure threshold in Pa
void setPressureThreshold(uint32_t p) {

}

//sets the reference pressure in Pa
void setPressureReference(uint32_t p) {

}

//returns differential pressure in Pa
uint32_t getPressure(void) {

}

//sets sample rate to that necessary to take a breath sample
//disables threshold interrupt
void setPressureActive(void) {

}

//sets sample rate to that necessary for Nordic to sleep and
//enables the threshold interrupt
void setPressureInactive(void) {

}

