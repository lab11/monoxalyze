
#include <stdint.h>
#include "lps25h.h"
#include "twi_master.h"

#define PRESS_ADDRESS     0xB8

#define REF_P_XL    0x08
#define REF_P_L     0x09
#define REF_P_H     0x0A
#define RES_CONF    0x10
#define CTRL_REG1   0x20
#define CTRL_REG2   0x21
#define CTRL_REG3   0x22
#define CTRL_REG4   0x23
#define INT_CFG     0x24
#define INT_SOURCE  0x25
#define STATUS_REG  0x27
#define PRESS_OUT_XL   0x28
#define PRESS_OUT_L     0x29
#define PRESS_OUT_H     0x2A
#define TEMP_OUT_L		0x2B
#define TEMP_OUT_H		0x2C
#define FIFO_CTRL       0x2E
#define FIFO_STATUS     0x2F
#define THIS_P_L        0x30
#define THIS_P_H        0x31
#define RPDS_L          0x39
#define RPDS_H          0x3A


//initializes the pressure sensor to the above configuration
uint8_t pressureInit() {
    twi_master_init();
    setPressureActive();

	return 0;
}

//sets the interrupt pressure threshold in Pa
void setPressureThreshold(uint32_t p) {

}

//sets the reference pressure in Pa
void setPressureReference(uint32_t p) {

}

//returns differential pressure in Pa
uint32_t getPressure(void) {

    //write to the slave address to the subbaddress
	uint8_t toWrite[2] = {CTRL_REG2, 0x11};
	twi_master_transfer(PRESS_ADDRESS, toWrite, 2, true);

    //set MSB of subaddress to enable auto increment
    uint8_t subAddress[2] = {PRESS_OUT_XL | 0x80, 0x00};
    twi_master_transfer(PRESS_ADDRESS, subAddress, 1, false);

    //read three sequential pressures
    uint8_t press[3];
    twi_master_transfer(PRESS_ADDRESS | 0x01, press, 3, true);

    //calculate pressurea
    uint32_t pressPa = 0x00;
    pressPa = (press[2] << 16) | (press[1] << 8) | press[0];
    
    //return (int32_t)(pressPa/40.96);
	return pressPa;
}

uint16_t getTemp(void) {
	uint8_t toWrite[2] = {CTRL_REG2, 0x11};
	twi_master_transfer(PRESS_ADDRESS, toWrite, 2, true);

    //set MSB of subaddress to enable auto increment
    uint8_t subAddress[2] = {TEMP_OUT_L | 0x80, 0x00};
    twi_master_transfer(PRESS_ADDRESS, subAddress, 1, false);

    uint8_t temp[2];
    twi_master_transfer(PRESS_ADDRESS | 0x01, temp, 2, true);

    //calculate temp
    uint16_t tempOut = 0x00;
	tempOut = (temp[1] << 8) | temp[0];
    
	return tempOut;

}

//sets sample rate to that necessary to take a breath sample
//disables threshold interrupt
void setPressureActive(void) {
	uint8_t subAddress[2] = {CTRL_REG1, 0x80};
    twi_master_transfer(PRESS_ADDRESS, subAddress, 2, true);

}

//sets sample rate to that necessary for Nordic to sleep and
//enables the threshold interrupt
void setPressureInactive(void) {
    //enables interrupt on high pressure
    //lowers ODR to 1hz
	uint8_t subAddress[2] = {CTRL_REG1, 0x00};
    twi_master_transfer(PRESS_ADDRESS, subAddress, 2, true);

}

