#ifndef SI7021_H
#define SI7021_H

#define SI7021_HEATER_LEVEL_1 	0x00 	//3.09ma
#define SI7021_HEATER_LEVEL_2 	0x01 	//9.18ma
#define SI7021_HEATER_LEVEL_3 	0x02 	//15.24ma
#define SI7021_HEATER_LEVEL_4 	0x04 	//27.39ma
#define SI7021_HEATER_LEVEL_5 	0x08 	//51.69ma
#define SI7021_HEATER_LEVEL_6 	0x0F 	//94.2ma

float 		si7021GetTemperature(void);
uint32_t 	si7021GetHumidity(void);
void 		si7021SetHeaterLevel(uint8_t);
void 		si7021SetHeaterEnable(void);
void 		si7021SetHeaterDisable(void);

#endif
