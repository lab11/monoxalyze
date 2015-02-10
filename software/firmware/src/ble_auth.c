
#include "ble_auth.h"
#include <string.h>
#include "nordic_common.h"
#include "ble_srv_common.h"
#include "app_util.h"

static void on_connect(ble_auth_t* auth, ble_evt_t* ble_evt) {
		auth->conn_handle = ble_evt->evt.gap_evt.conn_handle;
}

static void on_disconnect(ble_auth_t* auth, ble_evt_t* ble_evt) {
	UNUSED_PARAMETER(ble_evt);
	auth->conn_handle = BLE_CONN_HANDLE_INVALID;
}

void ble_auth_on_ble_evt(ble_auth_t* auth, ble_evt_t* ble_evt) {

	switch(ble_evt->header.evt_id) {
		case BLE_GAP_EVT_CONNECTED:
			on_connect(auth, ble_evt);
			break;
		case BLE_GAP_EVT_DISCONNECTED:
			on_disconnect(auth, ble_evt);
			break;
		default:
			break;
	}
}



static uint32_t protocol_char_add(ble_auth_t* auth) {
	ble_gatts_char_md_t char_md;
	ble_gatts_attr_md_t cccd_md;
	ble_gatts_attr_t 	attr_char_value;
	ble_uuid_t 			ble_uuid;
	ble_gatts_attr_md_t attr_md;

	memset(&cccd_md, 0, sizeof(cccd_md));

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

	cccd_md.vloc = BLE_GATTS_VLOC_STACK;

	char_md.char_props.read = 1;
	char_md.char_props.write = 1;
	char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

	ble_uuid.type = auth->uuid_type;
	ble_uuid.uuid = AUTH_UUID_PROTOCOL_CHAR;

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
	attr_char_value.init_len	= sizeof(uint8_t);
	attr_char_value.init_offs	= 0;
	attr_char_value.max_len		= sizeof(uint8_t);
	attr_char_value.p_value		= NULL;

	return sd_ble_gatts_characteristic_add(auth->service_handle, &char_md,
														&attr_char_value,
														&auth->protocol_char_handles);
}

static uint32_t len_char_add(ble_auth_t* auth) {
	ble_gatts_char_md_t char_md;
	ble_gatts_attr_md_t cccd_md;
	ble_gatts_attr_t 	attr_char_value;
	ble_uuid_t 			ble_uuid;
	ble_gatts_attr_md_t attr_md;

	memset(&cccd_md, 0, sizeof(cccd_md));

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

	cccd_md.vloc = BLE_GATTS_VLOC_STACK;

	char_md.char_props.read = 1;
	char_md.char_props.write = 1;
	char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

	ble_uuid.type = auth->uuid_type;
	ble_uuid.uuid = AUTH_UUID_LEN_CHAR;

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
	attr_char_value.init_len	= sizeof(uint16_t);
	attr_char_value.init_offs	= 0;
	attr_char_value.max_len		= sizeof(uint16_t);
	attr_char_value.p_value		= NULL;

	return sd_ble_gatts_characteristic_add(auth->service_handle, &char_md,
														&attr_char_value,
														&auth->len_char_handles);
}

static uint32_t data_char_add(ble_auth_t* auth) {
	ble_gatts_char_md_t char_md;
	ble_gatts_attr_md_t cccd_md;
	ble_gatts_attr_t 	attr_char_value;
	ble_uuid_t 			ble_uuid;
	ble_gatts_attr_md_t attr_md;

	memset(&cccd_md, 0, sizeof(cccd_md));

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

	cccd_md.vloc = BLE_GATTS_VLOC_STACK;

	char_md.char_props.read = 1;
	char_md.char_props.write = 1;
	char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

	ble_uuid.type = auth->uuid_type;
	ble_uuid.uuid = AUTH_UUID_DATA_CHAR;

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
	attr_char_value.init_len	= sizeof(uint16_t);
	attr_char_value.init_offs	= 0;
	attr_char_value.max_len		= sizeof(uint16_t);
	attr_char_value.p_value		= NULL;

	return sd_ble_gatts_characteristic_add(auth->service_handle, &char_md,
														&attr_char_value,
														&auth->data_char_handles);
}

static uint32_t pass_char_add(ble_auth_t* auth) {
	ble_gatts_char_md_t char_md;
	ble_gatts_attr_md_t cccd_md;
	ble_gatts_attr_t 	attr_char_value;
	ble_uuid_t 			ble_uuid;
	ble_gatts_attr_md_t attr_md;

	memset(&cccd_md, 0, sizeof(cccd_md));

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

	cccd_md.vloc = BLE_GATTS_VLOC_STACK;

	char_md.char_props.read = 1;
	char_md.char_props.write = 1;
	char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

	ble_uuid.type = auth->uuid_type;
	ble_uuid.uuid = AUTH_UUID_PASS_CHAR;

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
	attr_char_value.init_len	= sizeof(uint8_t);
	attr_char_value.init_offs	= 0;
	attr_char_value.max_len		= sizeof(uint8_t);
	attr_char_value.p_value		= NULL;

	return sd_ble_gatts_characteristic_add(auth->service_handle, &char_md,
														&attr_char_value,
														&auth->pass_char_handles);
}

static uint32_t retry_char_add(ble_auth_t* auth) {
	ble_gatts_char_md_t char_md;
	ble_gatts_attr_md_t cccd_md;
	ble_gatts_attr_t 	attr_char_value;
	ble_uuid_t 			ble_uuid;
	ble_gatts_attr_md_t attr_md;

	memset(&cccd_md, 0, sizeof(cccd_md));

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

	cccd_md.vloc = BLE_GATTS_VLOC_STACK;

	char_md.char_props.read = 1;
	char_md.char_props.write = 1;
	char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

	ble_uuid.type = auth->uuid_type;
	ble_uuid.uuid = AUTH_UUID_RETRY_CHAR;

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
	attr_char_value.init_len	= sizeof(uint8_t);
	attr_char_value.init_offs	= 0;
	attr_char_value.max_len		= sizeof(uint8_t);
	attr_char_value.p_value		= NULL;

	return sd_ble_gatts_characteristic_add(auth->service_handle, &char_md,
														&attr_char_value,
														&auth->retry_char_handles);
}

static uint32_t start_char_add(ble_auth_t* auth) {
	ble_gatts_char_md_t char_md;
	ble_gatts_attr_md_t cccd_md;
	ble_gatts_attr_t 	attr_char_value;
	ble_uuid_t 			ble_uuid;
	ble_gatts_attr_md_t attr_md;

	memset(&cccd_md, 0, sizeof(cccd_md));

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

	cccd_md.vloc = BLE_GATTS_VLOC_STACK;

	char_md.char_props.read = 1;
	char_md.char_props.write = 1;
	char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

	ble_uuid.type = auth->uuid_type;
	ble_uuid.uuid = AUTH_UUID_START_CHAR;

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
	attr_char_value.init_len	= sizeof(uint8_t);
	attr_char_value.init_offs	= 0;
	attr_char_value.max_len		= sizeof(uint8_t);
	attr_char_value.p_value		= NULL;

	return sd_ble_gatts_characteristic_add(auth->service_handle, &char_md,
														&attr_char_value,
														&auth->start_char_handles);
}

static uint32_t accept_char_add(ble_auth_t* auth) {
	ble_gatts_char_md_t char_md;
	ble_gatts_attr_md_t cccd_md;
	ble_gatts_attr_t 	attr_char_value;
	ble_uuid_t 			ble_uuid;
	ble_gatts_attr_md_t attr_md;

	memset(&cccd_md, 0, sizeof(cccd_md));

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

	cccd_md.vloc = BLE_GATTS_VLOC_STACK;

	char_md.char_props.read = 1;
	char_md.char_props.notify = 1;
	char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

	ble_uuid.type = auth->uuid_type;
	ble_uuid.uuid = AUTH_UUID_ACCEPT_CHAR;

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
	attr_char_value.init_len	= sizeof(uint8_t);
	attr_char_value.init_offs	= 0;
	attr_char_value.max_len		= sizeof(uint8_t);
	attr_char_value.p_value		= NULL;

	return sd_ble_gatts_characteristic_add(auth->service_handle, &char_md,
														&attr_char_value,
														&auth->accept_char_handles);
}

uint32_t ble_auth_init(ble_auth_t* auth, const ble_auth_init_t* auth_init) {
	uint32_t err_code;
	ble_uuid_t ble_uuid;

	auth->conn_handle				= BLE_CONN_HANDLE_INVALID;
	auth->protocol_write_handler	= auth_init->protocol_write_handler;
	auth->len_write_handler			= auth_init->len_write_handler;
	auth->data_write_handler		= auth_init->data_write_handler;
	auth->start_write_handler		= auth_init->start_write_handler;
	auth->pass_write_handler		= auth_init->pass_write_handler;
	auth->retry_write_handler		= auth_init->retry_write_handler;

	ble_uuid128_t base_uuid = {AUTH_UUID_BASE};
	err_code = sd_ble_uuid_vs_add(&base_uuid, &auth->uuid_type);
	if (err_code != NRF_SUCCESS) {
		return err_code;
	}

	ble_uuid.type = auth->uuid_type;
	ble_uuid.uuid = AUTH_UUID_SERVICE;

	err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid,
											&auth->service_handle);
	if(err_code != NRF_SUCCESS) {
		return err_code;
	}

	err_code = protocol_char_add(auth);
	if(err_code != NRF_SUCCESS) {
		return err_code;
	}

	err_code = len_char_add(auth);
	if(err_code != NRF_SUCCESS) {
		return err_code;
	}

	err_code = data_char_add(auth);
	if(err_code != NRF_SUCCESS) {
		return err_code;
	}
	
	err_code = start_char_add(auth);
	if(err_code != NRF_SUCCESS) {
		return err_code;
	}

	err_code = accept_char_add(auth);
	if(err_code != NRF_SUCCESS) {
		return err_code;
	}
	
	err_code = pass_char_add(auth);
	if(err_code != NRF_SUCCESS) {
		return err_code;
	}
	
	err_code = retry_char_add(auth);
	if(err_code != NRF_SUCCESS) {
		return err_code;
	}

	return NRF_SUCCESS;
}

uint32_t ble_auth_on_accept_change(ble_auth_t* auth, uint8_t accept) {

    ble_gatts_hvx_params_t params;
    uint16_t len = sizeof(accept);

    memset(&params, 0, sizeof(params));
    params.type = BLE_GATT_HVX_NOTIFICATION;
    params.handle = auth->accept_char_handles.value_handle;
    params.p_data = &accept;
    params.p_len = &len;

    return sd_ble_gatts_hvx(auth->conn_handle, &params);  
}

