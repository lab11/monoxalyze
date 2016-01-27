#ifndef BLECONFIG_H
#define BLECONFIG_H

#define MONOXALYZE_SERVICE_COLLECT_SHORT_UUID		0x11AA
#define MONOXALYZE_CHAR_START_COLLECT_SHORT_UUID	0x11AB
#define MONOXALYZE_CHAR_STOP_COLLECT_SHORT_UUID		0x11AC
#define MONOXALYZE_CHAR_START_READ_SHORT_UUID		0x11AD
#define MONOXALYZE_CHAR_DONE_READ_SHORT_UUID		0x11AE
#define MONOXALYZE_CHAR_DATA_SHORT_UUID				0x11AF

#define DEVICE_NAME "Monoxalyze"

typedef struct ble_monoxalyze_s {
	uint8_t		start_collect_val;
	uint8_t		stop_collect_val;
	uint8_t		start_read_val;
	uint8_t		done_read_val;
	uint8_t 	data_ptr[20];
	//uint8_t* 	data_ptr;
} ble_monoxalyze_t;

typedef enum state{                                                             
    ADVERTISING,                                                                
    WAITING,                                                                    
    COLLECTING,                                                                 
    DONE_COLLECTING,                                                            
    WRITING_BACK                                                                
} Device_state;

#endif
