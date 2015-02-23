
#include <stdint.h>
#include "pins.h"
#include "nrf.h"
#include "lmp91000.h"
#include "twi_master.h"
#include "adc.h"


//I2C address
#define GAS_ADDRESS 0x90

//register definition
#define TIACN       0x10
#define REFCN       0x11
#define MODECN      0x12
#define STATUS      0x00
#define LOCK        0x01

//values to set
#define TIA_GAIN    0x07
#define RLOAD       0x00

#define REF_SOURCE  0x01
#define INT_Z       0x00
#define BIAS_SIGN   0x01
#define BIAS        0x00


//lock and unlock register protection
static uint8_t ctrlUnlock(void);
static uint8_t ctrlLock(void);
static uint8_t setTIA(void);
static uint8_t setREF(void);
static uint8_t checkReady(void);

uint8_t gasInit() {

	twi_master_init();

	//while(!checkReady());

	uint8_t err = 1;
    err = ctrlUnlock();
    err = setTIA();
    err = setREF();
    //err = ctrlLock();

    err = setGasActive();

	if(err != 1)
		return 1;
	else
		return 0;
}

uint32_t getPPM() {
    adcConfig(ADC_CONFIG_RES_10bit, ADC_CONFIG_INPSEL_AnalogInputNoPrescaling,
                                    ADC_CONFIG_REFSEL_VBG,
                                    1 << VOUT,
                                    ADC_CONFIG_EXTREFSEL_None);

   	int16_t sample = (int16_t)getSample();
	sample -= 307; //ref
	sample -= 5; //linearity shift
	sample = (int16_t)(((float)sample)*.703);
	if(sample < 0) {
		sample = 0;
	}
	return sample;
}

uint16_t getTemp() {
    uint8_t down[2] = {MODECN, 0x06};
    twi_master_transfer(GAS_ADDRESS, down, 2, true);

    adcConfig(ADC_CONFIG_RES_10bit, ADC_CONFIG_INPSEL_AnalogInputOneThirdPrescaling,
                                    ADC_CONFIG_REFSEL_VBG,
                                    1 << VOUT,
                                    ADC_CONFIG_EXTREFSEL_None);
    
    uint16_t temp = getSample();

    //down[1] = 0x03;
    //twi_master_transfer(GAS_ADDRESS, down, 2, true);

    return temp;
}

uint8_t setGasInactive() {
    uint8_t down[2] = {MODECN, 0x00};
    return twi_master_transfer(GAS_ADDRESS, down, 2, true);
}

uint8_t setGasActive() {
    uint8_t down[2] = {MODECN, 0x03};
    return twi_master_transfer(GAS_ADDRESS, down, 2, true);
}

static uint8_t ctrlUnlock() {
    uint8_t unlock[2];
	unlock[0] = LOCK;
	unlock[1] = 0x00;
    return twi_master_transfer(GAS_ADDRESS, unlock, 2, true);
}

static uint8_t ctrlLock() {
    uint8_t lock[2] = {LOCK, 0x01};
    return twi_master_transfer(GAS_ADDRESS, lock, 2, true);
}

static uint8_t checkReady() {
	uint8_t status[2] = {STATUS, 0x00};
	twi_master_transfer(GAS_ADDRESS, status, 1, true);

	uint8_t read = 0xFF;
	twi_master_transfer(GAS_ADDRESS | 0x01, &read, 2, true);

	return read;

}

static uint8_t setTIA() {
    uint8_t tia[2];
    tia[0] = TIACN;
    //tia[1] = (TIA_GAIN << 2) | RLOAD;
	tia[1] = 0x1F;
    return twi_master_transfer(GAS_ADDRESS, tia, 2, true);
}

static uint8_t setREF() {
    uint8_t ref[2];
    ref[0] = REFCN;
    //ref[1] = (REF_SOURCE << 7) | (INT_Z << 5) | (BIAS_SIGN << 4) | BIAS;
	ref[1] = 0x90;
    return twi_master_transfer(GAS_ADDRESS, ref, 2, true);
}

