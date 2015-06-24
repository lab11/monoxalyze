
#include <stdint.h>
#include "si7021.h"
#include "twi_master.h"

#define SI7021_ADDR	0x80

#define SI7021_RH_HOLD 		0xE5
#define SI7021_RH_NOHOLD 	0xF5
#define SI7021_TEMP_HOLD 	0xE3
#define SI7021_TEMP_NOHOLD 	0xF3
#define SI7021_CTRL_WRITE	0xE6
#define SI7021_CTRL_READ	0xE7
#define SI7021_HEATER_WRITE	0x51
#define SI7021_HEATER_READ  0x11

#define SI7021_HEATER_ENABLE 0x04
#define SI7021_HEATER_DISABLE 0x00

float si7021GetTemperature(void) {
	uint8_t temp_hum_write[] = {SI7021_TEMP_HOLD};
	twi_master_transfer(SI7021_ADDR, temp_hum_write, 
						sizeof(temp_hum_write), TWI_DONT_ISSUE_STOP);

	uint8_t temp_hum_data[3];
	twi_master_transfer(SI7021_ADDR | 0x01, temp_hum_data,
						sizeof(temp_hum_data),TWI_ISSUE_STOP); 

	float temperature = (-46.85 + (175.72 * (((uint32_t) temp_hum_data[0] << 8) 
							| ((uint32_t) temp_hum_data[1] & 0xfc)) / (1 << 16)));	
	return temperature;	
}
 
uint32_t si7021GetHumidity(void) {
				
	uint8_t temp_hum_write[] = {SI7021_RH_HOLD};

	twi_master_transfer(SI7021_ADDR, temp_hum_write,
						 sizeof(temp_hum_write), TWI_DONT_ISSUE_STOP);

	uint8_t temp_hum_data[3];
	twi_master_transfer(SI7021_ADDR | 0x01, temp_hum_data,
						sizeof(temp_hum_data),TWI_ISSUE_STOP); 

	float humidity = -6.0 + ((125.0 / (1 << 16)) * (((uint32_t) temp_hum_data[0] << 8) 
										| ((uint32_t) temp_hum_data[1] & 0xf0)));
	if(humidity < 0) {
		humidity = 0;
	}
	else if(humidity > 100) {
		humidity = 100;
	}

	return (uint32_t)humidity;
}

void si7021SetHeaterLevel(uint8_t heaterLevel) {
	uint8_t temp_hum_write[] = {SI7021_HEATER_WRITE, heaterLevel};

	twi_master_transfer(SI7021_ADDR, temp_hum_write,
						 sizeof(temp_hum_write), TWI_ISSUE_STOP);
}

void si7021SetHeaterEnable(void) {
	uint8_t temp_hum_write[] = {SI7021_CTRL_WRITE, SI7021_HEATER_ENABLE};

	twi_master_transfer(SI7021_ADDR, temp_hum_write,
						 sizeof(temp_hum_write), TWI_ISSUE_STOP);

}

void si7021SetHeaterDisable(void) {

	uint8_t temp_hum_write[] = {SI7021_CTRL_WRITE, SI7021_HEATER_DISABLE};

	twi_master_transfer(SI7021_ADDR, temp_hum_write,
						 sizeof(temp_hum_write), TWI_ISSUE_STOP);
}
