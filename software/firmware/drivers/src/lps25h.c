
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
#define THS_P_L        0x30
#define THS_P_H        0x31
#define RPDS_L          0x39
#define RPDS_H          0x3A

static void setupPressure();

//initializes the pressure sensor to the above configuration
uint8_t pressureInit() {
    twi_master_init();
	setupPressure();
    setPressureActive();

	return 0;
}

//sets the interrupt pressure threshold in Pa
void setPressureThreshold(uint32_t p) {
	p = p/100;
	p = p*16;

	uint8_t toWrite[2] = {THS_P_L, p&0x000000FF};
	twi_master_transfer(PRESS_ADDRESS, toWrite, 2, true);

	toWrite[0] = THS_P_H;
	toWrite[1] = ((p & 0x0000FF00)>>8);
	twi_master_transfer(PRESS_ADDRESS, toWrite, 2, true);
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
    
    return (uint32_t)pressPa/40.96;
	//return pressPa;
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

static void setupPressure(void) {

	//ENABLE, ODR 25Hz, INTERUPT DISABLED
	uint8_t ctr1[2] = {CTRL_REG1, 0xC0};
    twi_master_transfer(PRESS_ADDRESS, ctr1, 2, true);

	//enable FIFO
	uint8_t ctr2[2] = {CTRL_REG2, 0x60};
    twi_master_transfer(PRESS_ADDRESS, ctr2, 2, true);

	//set interrupt as a pressure high interrupt
	uint8_t ctr3[2] = {CTRL_REG3, 0x01};
    twi_master_transfer(PRESS_ADDRESS, ctr3, 2, true);

	//noctrl4 changes


	//pressure high interrupt
	uint8_t intcfg[2] = {INT_CFG, 0x01};
    twi_master_transfer(PRESS_ADDRESS, intcfg, 2, true);

	//mean mode 2 sample moving average
	uint8_t fifoctl[2] = {FIFO_CTRL, 0xC1};
    twi_master_transfer(PRESS_ADDRESS, fifoctl, 2, true);
}

//sets sample rate to that necessary to take a breath sample
//disables threshold interrupt
void setPressureActive(void) {
	uint8_t subAddress[2] = {CTRL_REG1, 0xC0};
    twi_master_transfer(PRESS_ADDRESS, subAddress, 2, true);

}

//sets sample rate to that necessary for Nordic to sleep and
//enables the threshold interrupt
void setPressureInactive(void) {
	
    //enables interrupt on high pressure
    //lowers ODR to 1hz
	uint8_t subAddress[2] = {CTRL_REG1, 0x98};
    twi_master_transfer(PRESS_ADDRESS, subAddress, 2, true);

}

