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

typedef struct ble_co_s ble_co_t;

typedef struct ble_co_s {
	uint16_t					service_handle;
	ble_gatts_char_handles_t	gas_char_handles;
	ble_gatts_char_handles_t	press_char_handles;
	uint8_t						uuid_type;
	uint16_t					conn_handle;
	bool						is_gas_notifying;
	bool						is_press_notifying;
} ble_co_t;

uint32_t ble_co_init(ble_co_t* co);

void ble_co_on_ble_evt(ble_co_t* co, ble_evt_t* ble_evt);

uint32_t ble_co_on_gas_change(ble_co_t* co, uint32_t gas);
uint32_t ble_co_on_press_changle(ble_co_t* co, uint32_t press);

#endif
