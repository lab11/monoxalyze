
#include <stdint.h>
#include "pins.h"
#include "nrf.h"
#include "lmp91000.h"
#include "twi_master.h"
#include "adc.h"


//I2C address
#define ADDRESS 0x90

//register definition
#define TIACN       0x10
#define REFCN       0x11
#define MODECN      0x12
#define STATUS      0x00
#define LOCK        0x01

//values to set
#define TIA_GAIN    7
#define RLOAD       0

#define REF_SOURCE  1
#define INT_Z       0
#define BIAS_SIGN   0
#define BIAS        0


//lock and unlock register protection
static void ctrlUnlock(void);
static void ctrlLock(void);
static void setTIA(void);
static void setREF(void);

uint8_t gasInit() {

    ctrlUnlock();
    setTIA();
    setREF();
    ctrlLock();

    setGasActive();
    return 0;
}

uint16_t getPPM() {
    adcConfig(ADC_CONFIG_RES_10bit, ADC_CONFIG_INPSEL_AnalogInputNoPrescaling,
                                    ADC_CONFIG_REFSEL_VBG,
                                    1 << VOUT,
                                    ADC_CONFIG_EXTREFSEL_None);
    return getSample();
}

uint16_t getTemp() {
    uint8_t down[2] = {MODECN, 0x06};
    twi_master_transfer(ADDRESS, down, 2, true);

    adcConfig(ADC_CONFIG_RES_10bit, ADC_CONFIG_INPSEL_AnalogInputNoPrescaling,
                                    ADC_CONFIG_REFSEL_VBG,
                                    1 << VOUT,
                                    ADC_CONFIG_EXTREFSEL_None);
    
    uint16_t temp = getSample();

    down[1] = 0x03;
    twi_master_transfer(ADDRESS, down, 2, true);

    return temp;
}

void setGasInactive() {
    uint8_t down[2] = {MODECN, 0x00};
    twi_master_transfer(ADDRESS, down, 2, true);
}

void setGasActive() {
    uint8_t down[2] = {MODECN, 0x03};
    twi_master_transfer(ADDRESS, down, 2, true);
}

static void ctrlUnlock() {
    uint8_t unlock = 0x00;
    twi_master_transfer(ADDRESS, &unlock, 1, true);
}

static void ctrlLock() {
    uint8_t lock = 0x00;
    twi_master_transfer(ADDRESS, &lock, 1, true);
}

static void setTIA() {
    uint8_t tia[2];
    tia[0] = TIACN;
    tia[1] = (TIA_GAIN << 2) & RLOAD;
    twi_master_transfer(ADDRESS, tia, 2, true);
}

static void setREF() {
    uint8_t ref[2];
    ref[0] = REFCN;
    ref[1] = (REF_SOURCE << 7) & (INT_Z << 5) & (BIAS_SIGN << 4) & BIAS;
    twi_master_transfer(ADDRESS, ref, 2, true);
}

