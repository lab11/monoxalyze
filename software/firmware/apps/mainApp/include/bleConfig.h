#ifndef BLECONFIG_H
#define BLECONFIG_H

//#define BLE_UUID_GAS_SERVICE 2
//#define BLE_UUID_LED_SERVICE 1

//cannot change server database if -
#define IS_SRVC_CHANGED_CHARACT_PRESENT 1

#define DEVICE_NAME "Monoxalyze"

//advertising intervale = 64*0.625ms = 40ms
#define APP_ADV_INTERVAL                MSEC_TO_UNITS(20, UNIT_0_625_MS)

#define APP_COMPANY_IDENTIFIER			0x11BB

//advertising timeout sec
#define APP_ADV_TIMEOUT_IN_SECONDS      20

//RTC1_Prescale
#define APP_TIMER_PRESCALER             50

#define APP_TIMER_MAX_TIMERS            4

//size of op queues
#define APP_TIMER_OP_QUEUE_SIZE         20

#define NOTIFY_RATE						APP_TIMER_TICKS(50, APP_TIMER_PRESCALER)


//500ms
#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(10, UNIT_1_25_MS)

//1s
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(1000, UNIT_1_25_MS)

#define SLAVE_LATENCY                   0

//supervisory timeout 4s
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)

//time from initiating event(conn or notify start) to first time param_update c
//called
#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER)


#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000, APP_TIMER_PRESCALER)

//attempts before giving up parameter negotiation
#define MAX_CONN_PARAMS_UPDATE_COUNT    3

//timeout for pairing or sec requests secs
#define SEC_PARAM_TIMEOUT               30

//perform bonding
#define SEC_PARAM_BOND                  1

//no man in the middle
#define SEC_PARAM_MITM                  0

//no i/o capability
#define SEC_PARAM_IO_CAPABILITIES       BLE_GAP_IO_CAPS_NONE

//no out of bound data
#define SEC_PARAM_OOB                   0

#define SEC_PARAM_MIN_KEY_SIZE          7

#define SEC_PARAM_MAX_KEY_SIZE          16

#define DEAD_BEEF                       0xDEADBEEF

//max scheduler event size
#define SCHED_MAX_EVENT_DATA_SIZE       sizeof(app_timer_event_t)

//max num in scheduler queue
#define SCHED_QUEUE_SIZE                10

#endif
