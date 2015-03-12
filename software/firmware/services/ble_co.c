
#include "ble_co.h"
#include <string.h>
#include "nordic_common.h"
#include "ble_srv_common.h"
#include "app_util.h"

static void on_connect(ble_co_t* co, ble_evt_t* ble_evt) {
		co->conn_handle = ble_evt->evt.gap_evt.conn_handle;
}

static void on_disconnect(ble_co_t* co, ble_evt_t* ble_evt) {
	UNUSED_PARAMETER(ble_evt);
	co->conn_handle = BLE_CONN_HANDLE_INVALID;
}

static void on_gas_cccd_write(ble_co_t* co, ble_gatts_evt_write_t* evt_write) {

	if(evt_write->len == 2) {
		if(co->evt_handler != NULL) {

			ble_co_evt_t evt;

			if(ble_srv_is_notification_enabled(evt_write->data)) {
				evt.evt_type = BLE_CO_EVT_NOTIFICATION_ENABLED;
			} else {
				evt.evt_type = BLE_CO_EVT_NOTIFICATION_DISABLED;
			}

			co->evt_handler(co, &evt);
		}
	}
}

static void on_press_cccd_write(ble_co_t* co, ble_gatts_evt_write_t* evt_write) {
	if(evt_write->len == 2) {
		if(co->evt_handler != NULL) {
			ble_co_evt_t evt;

			if(ble_srv_is_notification_enabled(evt_write->data)) {
				evt.evt_type = BLE_CO_EVT_NOTIFICATION_ENABLED;
			} else {
				evt.evt_type = BLE_CO_EVT_NOTIFICATION_DISABLED;
			}

			co->evt_handler(co, &evt);
		}
	}
}

static void on_write(ble_co_t* co, ble_evt_t* ble_evt) {
	ble_gatts_evt_write_t* evt_write = &ble_evt->evt.gatts_evt.params.write;

	if(evt_write->handle == co->gas_char_handles.cccd_handle) {
		on_gas_cccd_write(co, evt_write);
	} else if (evt_write->handle == co->press_char_handles.cccd_handle) {
		on_press_cccd_write(co, evt_write);
	}

}

void ble_co_on_ble_evt(ble_co_t* co, ble_evt_t* ble_evt) {

	switch(ble_evt->header.evt_id) {
		case BLE_GAP_EVT_CONNECTED:
			on_connect(co, ble_evt);
			break;
		case BLE_GAP_EVT_DISCONNECTED:
			on_disconnect(co, ble_evt);
			break;
		case BLE_GATTS_EVT_WRITE:
			on_write(co, ble_evt);
			break;
		default:
			break;
	}
}



static uint32_t gas_char_add(ble_co_t* co, const ble_co_init_t * co_init) {

	ble_gatts_char_md_t char_md;
	ble_gatts_attr_md_t cccd_md;
	ble_gatts_attr_t 	attr_char_value;
	ble_uuid_t 			ble_uuid;
	ble_gatts_attr_md_t attr_md;

	memset(&cccd_md, 0, sizeof(cccd_md));

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	cccd_md.write_perm = co_init->co_gas_attr_md.cccd_write_perm;
	cccd_md.vloc = BLE_GATTS_VLOC_STACK;

	char_md.char_props.read = 1;
	char_md.char_props.notify = 1;
	char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

	ble_uuid.type = co->uuid_type;
	ble_uuid.uuid = CO_UUID_GAS_CHAR;

	memset(&attr_md, 0, sizeof(attr_md));

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

	attr_md.vloc		= BLE_GATTS_VLOC_STACK;
	attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
	attr_md.vlen       = 1;

	memset(&attr_char_value, 0, sizeof(attr_char_value));

	attr_char_value.p_uuid		= &ble_uuid;
	attr_char_value.p_attr_md	= &attr_md;
	attr_char_value.init_len	= sizeof(uint32_t);
	attr_char_value.init_offs	= 0;
	attr_char_value.max_len		= sizeof(uint32_t);
	attr_char_value.p_value		= NULL;

	return sd_ble_gatts_characteristic_add(co->service_handle, &char_md,
														&attr_char_value,
														&co->gas_char_handles);
}

static uint32_t press_char_add(ble_co_t* co, ble_co_init_t* co_init) {
	ble_gatts_char_md_t char_md;
	ble_gatts_attr_md_t cccd_md;
	ble_gatts_attr_t 	attr_char_value;
	ble_uuid_t 			ble_uuid;
	ble_gatts_attr_md_t attr_md;

	memset(&cccd_md, 0, sizeof(cccd_md));

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
	cccd_md.write_perm = co_init->co_gas_attr_md.cccd_write_perm;
	cccd_md.vloc = BLE_GATTS_VLOC_STACK;

	char_md.char_props.read = 1;
	char_md.char_props.notify = 1;
	char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

	ble_uuid.type = co->uuid_type;
	ble_uuid.uuid = CO_UUID_PRESS_CHAR;

	memset(&attr_md, 0, sizeof(attr_md));

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

	attr_md.vloc		= BLE_GATTS_VLOC_STACK;
	attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
	attr_md.vlen       = 0;

	memset(&attr_char_value, 0, sizeof(attr_char_value));

	attr_char_value.p_uuid		= &ble_uuid;
	attr_char_value.p_attr_md	= &attr_md;
	attr_char_value.init_len	= sizeof(uint32_t);
	attr_char_value.init_offs	= 0;
	attr_char_value.max_len		= sizeof(uint32_t);
	attr_char_value.p_value		= NULL;

	return sd_ble_gatts_characteristic_add(co->service_handle, &char_md,
														&attr_char_value,
														&co->press_char_handles);
}

uint32_t ble_co_init(ble_co_t* co, const ble_co_init_t* co_init) {
	uint32_t err_code;
	ble_uuid_t ble_uuid;

	co->conn_handle		= BLE_CONN_HANDLE_INVALID;

	ble_uuid128_t base_uuid = {CO_UUID_BASE};
	err_code = sd_ble_uuid_vs_add(&base_uuid, &co->uuid_type);
	if (err_code != NRF_SUCCESS) {
		return err_code;
	}

	ble_uuid.type = co->uuid_type;
	ble_uuid.uuid = CO_UUID_SERVICE;

	err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid,
											&co->service_handle);
	if(err_code != NRF_SUCCESS) {
		return err_code;
	}

	err_code = gas_char_add(co, co_init);
	if(err_code != NRF_SUCCESS) {
		return err_code;
	}

	err_code = press_char_add(co, co_init);
	if(err_code != NRF_SUCCESS) {
		return err_code;
	}

	return NRF_SUCCESS;
}

uint32_t ble_co_on_gas_change(ble_co_t* co, uint32_t gas) {

    ble_gatts_hvx_params_t params;
    uint16_t len = sizeof(gas);

    memset(&params, 0, sizeof(params));
    params.type = BLE_GATT_HVX_NOTIFICATION;
    params.handle = co->gas_char_handles.value_handle;
    params.p_data = &gas;
    params.p_len = &len;

    return sd_ble_gatts_hvx(co->conn_handle, &params);  
}

uint32_t ble_co_on_press_change(ble_co_t* co, uint32_t press) {

    ble_gatts_hvx_params_t params;
    uint16_t len = sizeof(press);

    memset(&params, 0, sizeof(params));
    params.type = BLE_GATT_HVX_NOTIFICATION;
    params.handle = co->press_char_handles.value_handle;
    params.p_data = &press;
    params.p_len = &len;

    return sd_ble_gatts_hvx(co->conn_handle, &params);  
}
