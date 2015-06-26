
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

static void on_humidity_cccd_write(ble_co_t* co, ble_gatts_evt_write_t* evt_write) {
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
static void on_temp_cccd_write(ble_co_t* co, ble_gatts_evt_write_t* evt_write) {
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
	} else if (evt_write->handle == co->temp_char_handles.cccd_handle) {
		on_temp_cccd_write(co, evt_write);
	} else if (evt_write->handle == co->humidity_char_handles.cccd_handle) {
		on_humidity_cccd_write(co, evt_write);
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

	BLE_UUID_BLE_ASSIGN(ble_uuid, CO_UUID_GAS_CHAR);

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

static uint32_t humidity_char_add(ble_co_t* co, const ble_co_init_t * co_init) {

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


	BLE_UUID_BLE_ASSIGN(ble_uuid, CO_UUID_HUMIDITY_CHAR);

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
														&co->humidity_char_handles);
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
	cccd_md.write_perm = co_init->co_press_attr_md.cccd_write_perm;
	cccd_md.vloc = BLE_GATTS_VLOC_STACK;

	char_md.char_props.read = 1;
	char_md.char_props.notify = 1;
	char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

	BLE_UUID_BLE_ASSIGN(ble_uuid, CO_UUID_PRESS_CHAR);

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

static uint32_t temp_char_add(ble_co_t* co, ble_co_init_t* co_init) {
	ble_gatts_char_md_t char_md;
	ble_gatts_attr_md_t cccd_md;
	ble_gatts_attr_t 	attr_char_value;
	ble_uuid_t 			ble_uuid;
	ble_gatts_attr_md_t attr_md;

	memset(&cccd_md, 0, sizeof(cccd_md));

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
	cccd_md.write_perm = co_init->co_temp_attr_md.cccd_write_perm;
	cccd_md.vloc = BLE_GATTS_VLOC_STACK;

	char_md.char_props.read = 1;
	char_md.char_props.notify = 1;
	char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

	BLE_UUID_BLE_ASSIGN(ble_uuid, CO_UUID_TEMP_CHAR);

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
	attr_char_value.init_len	= sizeof(float);
	attr_char_value.init_offs	= 0;
	attr_char_value.max_len		= sizeof(float);
	attr_char_value.p_value		= NULL;

	return sd_ble_gatts_characteristic_add(co->service_handle, &char_md,
														&attr_char_value,
														&co->temp_char_handles);
}
uint32_t ble_co_init(ble_co_t* co, const ble_co_init_t* co_init) {
	uint32_t err_code;
	ble_uuid_t ble_uuid;

	co->conn_handle		= BLE_CONN_HANDLE_INVALID;

	BLE_UUID_BLE_ASSIGN(ble_uuid, CO_UUID_SERVICE);

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

	err_code = humidity_char_add(co, co_init);
	if(err_code != NRF_SUCCESS) {
		return err_code;
	}

	err_code = temp_char_add(co, co_init);
	if(err_code != NRF_SUCCESS) {
		return err_code;
	}

	return NRF_SUCCESS;
}

uint32_t bleCoNotifyGas(ble_co_t* co, uint32_t gas) {

    ble_gatts_hvx_params_t params;
    uint16_t len = sizeof(gas);

    memset(&params, 0, sizeof(params));
    params.type = BLE_GATT_HVX_NOTIFICATION;
    params.handle = co->gas_char_handles.value_handle;
    params.p_data = &gas;
    params.p_len = &len;

    return sd_ble_gatts_hvx(co->conn_handle, &params);  
}

uint32_t bleCoNotifyTemp(ble_co_t* co, float temp) {

    ble_gatts_hvx_params_t params;
    uint16_t len = sizeof(temp);

    memset(&params, 0, sizeof(params));
    params.type = BLE_GATT_HVX_NOTIFICATION;
    params.handle = co->gas_char_handles.value_handle;
    params.p_data = &temp;
    params.p_len = &len;

    return sd_ble_gatts_hvx(co->conn_handle, &params);  
}

uint32_t bleCoUpdateGas(ble_co_t* co, uint32_t gas) {
	
	uint16_t len = sizeof(gas);

    return sd_ble_gatts_value_set(co->gas_char_handles.value_handle, 0, &len, &gas);  
}

uint32_t bleCoUpdateHumidity(ble_co_t* co, uint32_t humidity) {
	
	uint16_t len = sizeof(humidity);

    return sd_ble_gatts_value_set(co->humidity_char_handles.value_handle, 0, &len, &humidity);  
}

uint32_t bleCoUpdateTemp(ble_co_t* co, float temp) {
	
	uint16_t len = sizeof(temp);

    return sd_ble_gatts_value_set(co->gas_char_handles.value_handle, 0, &len, &temp);  
}

uint32_t bleCoNotifyPress(ble_co_t* co, uint32_t press) {

    ble_gatts_hvx_params_t params;
    uint16_t len = sizeof(press);

    memset(&params, 0, sizeof(params));
    params.type = BLE_GATT_HVX_NOTIFICATION;
    params.handle = co->press_char_handles.value_handle;
    params.p_data = &press;
    params.p_len = &len;

    return sd_ble_gatts_hvx(co->conn_handle, &params);  
}

uint32_t bleCoNotifyHumidity(ble_co_t* co, uint32_t humidity) {

    ble_gatts_hvx_params_t params;
    uint16_t len = sizeof(humidity);

    memset(&params, 0, sizeof(params));
    params.type = BLE_GATT_HVX_NOTIFICATION;
    params.handle = co->humidity_char_handles.value_handle;
    params.p_data = &humidity;
    params.p_len = &len;

    return sd_ble_gatts_hvx(co->conn_handle, &params);  
}
uint32_t bleCoUpdatePress(ble_co_t* co, uint32_t press) {
	
	uint16_t len = sizeof(press);
	
    return sd_ble_gatts_value_set(co->press_char_handles.value_handle, 0, &len, &press);  
}

