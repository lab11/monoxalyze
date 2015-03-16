
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "nordic_common.h"
#include "pins.h"
#include "led.h"
#include "nrf.h"
#include "lps25h.h"
#include "app_timer.h"

//8 hours worth of samples at 1 sample/sec
#define MAX_SAMPLE 28800

//timer stuff
#define TIMER_PRESCALER 	0
#define MAX_TIMERS 			1
#define TIMER_QUEUE_SIZE 	1
#define SAMPLE_RATE			APP_TIMER_TICKS(166, TIMER_PRESCALER)


static void sample(void* p_context);
static void timerInit(void);
static void timerStart(void);
static void writeWord(uint32_t* address, uint32_t value);

//local variables
static app_timer_id_t sampleTimer;
static uint32_t numSamples = 0;
static bool done = false;
static uint32_t* addr;
static uint32_t pg_size;
static uint32_t pg_num;
static uint32_t lastPressure;
static uint32_t currPressure;

static void sample (void* p_context) {
	static int i = 0;
	if(i%2){
		ledOn(LED_2);
	} else {
		ledOff(LED_2);
	}

	i++;
}

static void timerInit(void) {
	uint32_t err_code;

	APP_TIMER_INIT(TIMER_PRESCALER,
					MAX_TIMERS,
					TIMER_QUEUE_SIZE,
					false);

	err_code = app_timer_create(&sampleTimer,
								APP_TIMER_MODE_REPEATED,
								sample);

	APP_ERROR_CHECK(err_code);
}

static void timerStart(void) {
	uint32_t err_code;

	err_code = app_timer_start(sampleTimer, SAMPLE_RATE, NULL);
	APP_ERROR_CHECK(err_code);
}

static void clockInit(void) {
	NRF_CLOCK->LFCLKSRC = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_LFCLKSTART = 1;

    // Wait for the low frequency clock to start
    while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0) {}
    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
}


int main(void)
{
    // Initialize
	pressureInit();
	clockInit();
	timerInit();
	timerStart();
	ledInit();
	ledOn(LED_1);
	ledOn(LED_3);

    // Enter main loop
    while(true);
    {
		if(done) {
			app_timer_stop(sampleTimer);
		}

    }
}
