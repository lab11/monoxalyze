#ifndef BLE_CO_H
#define BLE_CO_H


#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"

#define CO_UUID_BASE {0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}
#define CO_UUID_SERVICE 0x1523
#define CO_UUID_GAS_CHAR 0x1525
#define CO_UUID_PRESS_CHAR 0x1524
#define CO_UUID_TEMP_CHAR 0x1526
#define CO_UUID_HUMIDITY_CHAR 0x1527

typedef enum {
	BLE_CO_EVT_NOTIFICATION_ENABLED,
    BLE_CO_EVT_NOTIFICATION_DISABLED
} ble_co_evt_type_t;

typedef struct
{
    ble_co_evt_type_t evt_type;
} ble_co_evt_t;

typedef struct ble_co_s ble_co_t;

typedef void (*ble_co_evt_handler_t) (ble_co_t* co, ble_co_evt_t* evt);


typedef struct {
	ble_co_evt_handler_t			evt_handler;
	ble_srv_cccd_security_mode_t	co_gas_attr_md;
	ble_srv_cccd_security_mode_t	co_press_attr_md;
	ble_srv_cccd_security_mode_t	co_temp_attr_md;
	ble_srv_cccd_security_mode_t	co_humidity_attr_md;
} ble_co_init_t;


typedef struct ble_co_s {
	ble_co_evt_handler_t		evt_handler;
	uint16_t					service_handle;
	ble_gatts_char_handles_t	gas_char_handles;
	ble_gatts_char_handles_t	press_char_handles;
	ble_gatts_char_handles_t	temp_char_handles;
	ble_gatts_char_handles_t	humidity_char_handles;
	uint8_t						uuid_type;
	uint16_t					conn_handle;
	bool						is_gas_notifying;
	bool						is_press_notifying;
	bool						is_temp_notifying;
	bool						is_humidity_notifying;
} ble_co_t;

uint32_t 	ble_co_init(ble_co_t* co, const ble_co_init_t* co_init);
void 		ble_co_on_ble_evt(ble_co_t* co, ble_evt_t* ble_evt);
uint32_t 	bleCoNotifyGas(ble_co_t* co, uint32_t gas);
uint32_t 	bleCoNotifyPress(ble_co_t* co, uint32_t press);
uint32_t 	bleCoNotifyTemp(ble_co_t* co, float temp);
uint32_t 	bleCoNotifyHumidity(ble_co_t* co, uint32_t temp);
uint32_t 	bleCoUpdateGas(ble_co_t* co, uint32_t gas);
uint32_t 	bleCoUpdatePress(ble_co_t* co, uint32_t press);
uint32_t 	bleCoUpdateTemp(ble_co_t* co, float temp);
uint32_t 	bleCoUpdateHumidity(ble_co_t* co, uint32_t temp);



#endif
