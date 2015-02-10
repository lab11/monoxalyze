#ifndef BLE_AUTH_H
#define BLE_AUTH_H

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"

#define AUTH_UUID_BASE {0x23, 0xD1, 0xDE, 0xEA, 0x3A, 0x78, 0x23, 0x27, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}
#define AUTH_UUID_SERVICE 0x1623
#define AUTH_UUID_PROTOCOL_CHAR 0x1624
#define AUTH_UUID_LEN_CHAR 0x1625
#define AUTH_UUID_DATA_CHAR 0x1626
#define AUTH_UUID_START_CHAR 0x162B
#define AUTH_UUID_ACCEPT_CHAR 0x1627
#define AUTH_UUID_PASS_CHAR 0x1629
#define AUTH_UUID_RETRY_CHAR 0x162A

typedef struct ble_auth_s ble_auth_t;

typedef void(*ble_auth_protocol_write_handler_t) 	(ble_auth_t* auth, uint8_t protocol);
typedef void(*ble_auth_len_write_handler_t) 		(ble_auth_t* auth, uint16_t len);
typedef void(*ble_auth_data_write_handler_t) 		(ble_auth_t* auth, uint8_t* dataArray);
typedef void(*ble_auth_start_write_handler_t) 		(ble_auth_t* auth, uint8_t start);
typedef void(*ble_auth_pass_write_handler_t) 		(ble_auth_t* auth, uint8_t pass);
typedef void(*ble_auth_retry_write_handler_t) 		(ble_auth_t* auth, uint8_t retry);

typedef struct {
	ble_auth_protocol_write_handler_t 	protocol_write_handler;
	ble_auth_len_write_handler_t 		len_write_handler;
	ble_auth_data_write_handler_t 		data_write_handler;
	ble_auth_start_write_handler_t 		start_write_handler;
	ble_auth_pass_write_handler_t 		pass_write_handler;
	ble_auth_retry_write_handler_t 		retry_write_handler;
} ble_auth_init_t;

typedef struct ble_auth_s {
	uint16_t					service_handle;
	ble_gatts_char_handles_t	protocol_char_handles;
	ble_gatts_char_handles_t	len_char_handles;
	ble_gatts_char_handles_t	data_char_handles;
	ble_gatts_char_handles_t	accept_char_handles;
	ble_gatts_char_handles_t	pass_char_handles;
	ble_gatts_char_handles_t	retry_char_handles;
	ble_gatts_char_handles_t	start_char_handles;
	uint8_t						uuid_type;
	uint16_t					conn_handle;
	bool						is_accept_notifying;
	ble_auth_protocol_write_handler_t 	protocol_write_handler;
	ble_auth_len_write_handler_t 		len_write_handler;
	ble_auth_data_write_handler_t 		data_write_handler;
	ble_auth_start_write_handler_t 		start_write_handler;
	ble_auth_pass_write_handler_t 		pass_write_handler;
	ble_auth_retry_write_handler_t 		retry_write_handler;

} ble_auth_t;

uint32_t ble_auth_init(ble_auth_t* auth, const ble_auth_init_t* auth_init);

void ble_auth_on_ble_evt(ble_auth_t* auth, ble_evt_t* ble_evt);

uint32_t ble_auth_on_accept_change(ble_auth_t* auth, uint8_t accept);

#endif
