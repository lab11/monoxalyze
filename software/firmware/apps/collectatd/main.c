#include <stdbool.h>
#include <stdint.h>
#include "bleConfig.h"
#include "nrf_gpio.h"
#include "ble_advdata.h"
#include "nordic_common.h"
#include "softdevice_handler.h"
#include "ble_debug_assert_handler.h"
#include "led.h"
#include "pins.h"
#include "nrf.h"
#include "nrf_log.h"
#include "lps25h.h"
#include "si7021.h"
#include "lmp91000.h"

#include "simple_ble.h"
#include "simple_timer.h"
#include "eddystone.h"
#include "simple_adv.h"

#define START_ADDR 0x24000//address to start writing data in flash
#define MAX_COLLECT 14400 // 3 minutes of 50ms samples

//*******************
//Variables for the logic of my app and writing back
//*******************
static Device_state state = ADVERTISING;
static uint32_t collect_count = 0;
static uint32_t notify_count = 0;
static uint8_t 	sent = 1;
static uint32_t pg_size;
static uint32_t pg_num;
static uint32_t* addr;

static void writeWord(uint32_t* , uint32_t );
static void storageInit(void);

//**********************
//variables for simpleBLE
//********************
static simple_ble_config_t ble_config = {
	.platform_id 	= 0x00,
	.device_id 		= DEVICE_ID_DEFAULT,
	.adv_name		= DEVICE_NAME,
	.adv_interval	= MSEC_TO_UNITS(100, UNIT_0_625_MS),
	.min_conn_interval = MSEC_TO_UNITS(10, UNIT_1_25_MS),
	.max_conn_interval = MSEC_TO_UNITS(15, UNIT_1_25_MS)
};

static ble_monoxalyze_t monoxalyze = {
	.start_collect_val = 0,
	.stop_collect_val = 0,
	.start_read_val = 0,
	.done_read_val = 0,
	.len_val = 0,
	.data_ptr = { 0 }
};

simple_ble_service_t monoxalyze_service = {
    .uuid128 = {{0x83, 0xbe, 0xd0, 0x58, 0xf1, 0xaf, 0x42, 0x62,
	             0x9c, 0xf8, 0x4f, 0x73, 0xe9, 0xdb, 0x9c, 0xe4}}
};
simple_ble_char_t start_collect_char =
						{.uuid16 = MONOXALYZE_CHAR_START_COLLECT_SHORT_UUID};
simple_ble_char_t stop_collect_char =
						{.uuid16 = MONOXALYZE_CHAR_STOP_COLLECT_SHORT_UUID};
simple_ble_char_t start_read_char =
						{.uuid16 = MONOXALYZE_CHAR_START_READ_SHORT_UUID};
simple_ble_char_t done_read_char =
						{.uuid16 = MONOXALYZE_CHAR_DONE_READ_SHORT_UUID};
simple_ble_char_t len_char =
						{.uuid16 = MONOXALYZE_CHAR_LEN_SHORT_UUID};
simple_ble_char_t data_char =
						{.uuid16 = MONOXALYZE_CHAR_DATA_SHORT_UUID};




void services_init (void) {

	simple_ble_add_service(&monoxalyze_service);

	simple_ble_add_characteristic(1,1,0,0,1, //read,write,len=1
								&monoxalyze.start_collect_val,
								&monoxalyze_service,
								&start_collect_char);

	simple_ble_add_characteristic(1,1,0,0,1, //read,write,len=1
								&monoxalyze.stop_collect_val,
								&monoxalyze_service,
								&stop_collect_char);

	simple_ble_add_characteristic(1,1,0,0,1, //read,write,len=1
								&monoxalyze.start_read_val,
								&monoxalyze_service,
								&start_read_char);

	simple_ble_add_characteristic(1,0,1,0,1, //read,notify,len=1
								&monoxalyze.done_read_val,
								&monoxalyze_service,
								&done_read_char);

	simple_ble_add_characteristic(1,0,0,0,4, //read,len=1
								(uint8_t*)&monoxalyze.len_val,
								&monoxalyze_service,
								&len_char);

	simple_ble_add_characteristic(1,1,1,1,20, //read,write,notify,vlen,len=1
								monoxalyze.data_ptr,
								&monoxalyze_service,
								&data_char);

}

static void collect_handler (void * p_context) {
	if(state == COLLECTING && collect_count < MAX_COLLECT) {
		//log_rtt_printf(0, "Handler called and collecting - taking samples\n");

		static uint32_t value[4]; 
		value[0] = getRawPressure();
		//log_rtt_printf(0, "got pressure data\n");
		//writeWord((uint32_t*)(addr+collect_count),value);
		log_rtt_printf(0, "wrote pressure data\n");

		value[1] = si7021GetTemperature();
		//log_rtt_printf(0, "got temp data\n");
		//writeWord((uint32_t*)(addr+collect_count),value);
		log_rtt_printf(0, "wrote temp data\n");

		value[2] = si7021GetHumidity();
		//log_rtt_printf(0, "got humidity data\n");
		//writeWord((uint32_t*)(addr+collect_count),value);
		log_rtt_printf(0, "wrote humidity data\n");
		
		value[3] = getGasSample();
		//log_rtt_printf(0, "got gas data\n");
		//writeWord((uint32_t*)(addr+collect_count),value);
		while(sd_flash_write((uint32_t* const)(addr+collect_count),(uint32_t const * const)value,4)
					!= NRF_SUCCESS);
		log_rtt_printf(0, "wrote gas data\n");
		collect_count += 4;
		monoxalyze.len_val = collect_count;

	} else if (state == WRITING_BACK) {
		//we are done with this read
		if(notify_count*20 >= collect_count*4 && sent == 1) {
			notify_count = 0;
			collect_count = 0;
			monoxalyze.done_read_val = 1;
			log_rtt_printf(0, "Sending done notification\n");
			simple_ble_notify_char(&done_read_char);
			state = DONE_COLLECTING;
			sent = 0;
			monoxalyze.len_val = 0;
		} else if(sent){
			log_rtt_printf(0, "Notifying data\n");
			//set the data in the characteristic
			memcpy(monoxalyze.data_ptr, (uint8_t*)(((uint8_t*)addr)+(notify_count*20)),20);
			log_rtt_printf(0, "This is my data %d\n", *(uint32_t*)monoxalyze.data_ptr);
			log_rtt_printf(0, "This is my data %d\n", *(uint32_t*)(monoxalyze.data_ptr+4));
			log_rtt_printf(0, "This is my data %d\n", *(uint32_t*)(monoxalyze.data_ptr+8));
			log_rtt_printf(0, "This is my data %d\n", *(uint32_t*)(monoxalyze.data_ptr+12));
			log_rtt_printf(0, "This is my data %d\n", *(uint32_t*)(monoxalyze.data_ptr+16));
			//monoxalyze.data_ptr = (uint8_t*)START_ADDR+(notify_count*20); // get byte addr
			simple_ble_notify_char(&data_char);
			sent = 0;
			notify_count++;
		} else {
			//log_rtt_printf(0, "not done sending previous packet\n");
		}

	}
}

static void writeWord(uint32_t* address, uint32_t value) {
    NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Wen << NVMC_CONFIG_WEN_Pos);
    while (NRF_NVMC->READY == NVMC_READY_READY_Busy);

    *address = value;
    while (NRF_NVMC->READY == NVMC_READY_READY_Busy);

    // Turn off flash write enable and wait until the NVMC is ready:
    NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Ren << NVMC_CONFIG_WEN_Pos);
    while (NRF_NVMC->READY == NVMC_READY_READY_Busy);
}

void ble_evt_connected(ble_evt_t* p_ble_evt) {
	log_rtt_printf(0, "Connected\n");
}

void ble_evt_disconnected(ble_evt_t* p_ble_evt) {
	log_rtt_printf(0, "Disonnected\n");
}

void ble_evt_tx_complete(ble_evt_t* p_ble_evt) {
	log_rtt_printf(0, "transmit complete\n");
	sent = 1;
}


void ble_evt_write(ble_evt_t* p_ble_evt) {
	if (simple_ble_is_char_event(p_ble_evt, &start_collect_char)) {
		log_rtt_printf(0, "Write to start collecting - starting\n");
		state = COLLECTING;
	} else if (simple_ble_is_char_event(p_ble_evt, &stop_collect_char)) {
		log_rtt_printf(0, "Write to stop collecting - stopping\n");
		state = ADVERTISING;
	} else if (simple_ble_is_char_event(p_ble_evt, &start_read_char)) {
		log_rtt_printf(0, "Write to start reading - starting\n");
		state = WRITING_BACK;
	}
}

static void storageInit(void) {
	pg_size = NRF_FICR->CODEPAGESIZE;
	pg_num = START_ADDR/pg_size;

	addr = (uint32_t*)(pg_size*pg_num);
}

int main(void) {

	log_rtt_init();
	log_rtt_printf(0, "Start RTT debugging\n");


	log_rtt_printf(0, "Initializing Storage Subsystem\n");
	storageInit();
	
	log_rtt_printf(0, "Initializing BLE\n");
	simple_ble_init(&ble_config);

	log_rtt_printf(0, "Initializing Timer\n");
	simple_timer_start(50, collect_handler);

	log_rtt_printf(0, "Starting Eddystone Advertising\n");
	eddystone_adv("goo.gl/nCQV8C",NULL);
	//simple_adv_only_name();

	while(1) {
		power_manage();
	}
}
