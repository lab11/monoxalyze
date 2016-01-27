PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
# Various C libraries that need to be included
APPLICATION_SRCS += softdevice_handler.c
APPLICATION_SRCS += ble_advdata.c
APPLICATION_SRCS += ble_conn_params.c
APPLICATION_SRCS += app_timer.c
APPLICATION_SRCS += ble_srv_common.c
APPLICATION_SRCS += app_util_platform.c
APPLICATION_SRCS += nrf_drv_common.c
APPLICATION_SRCS += nrf_drv_uart.c
APPLICATION_SRCS += nrf_delay.c
APPLICATION_SRCS += nrf_log.c
APPLICATION_SRCS += SEGGER_RTT.c
APPLICATION_SRCS += SEGGER_RTT_printf.c
APPLICATION_SRCS += led.c
APPLICATION_SRCS += lps25h.c
APPLICATION_SRCS += si7021.c
APPLICATION_SRCS += lmp91000.c
APPLICATION_SRCS += adc.c
APPLICATION_SRCS += twi_hw_master.c

APPLICATION_SRCS += simple_ble.c
APPLICATION_SRCS += simple_adv.c
APPLICATION_SRCS += eddystone.c
APPLICATION_SRCS += simple_timer.c
# Add other libraries here!

# platform-level headers and source files
LIBRARY_PATHS += $(NRF_BASE_PATH)/sdk/nrf51_sdk_10.0.0/components/drivers_nrf/twi_master/deprecated
LIBRARY_PATHS += $(NRF_BASE_PATH)/sdk/nrf51_sdk_10.0.0/components/drivers_nrf/pstorage/deprecated
LIBRARY_PATHS += ../../include
LIBRARY_PATHS += .
SOURCE_PATHS += $(NRF_BASE_PATH)/sdk/nrf51_sdk_10.0.0/components/drivers_nrf/twi_master/deprecated
SOURCE_PATHS += ../../src

CFLAGS += -DNRF_LOG_USES_RTT=1

# Set the softdevice needed for the application
SOFTDEVICE_MODEL = s110

# Include the main Makefile
NRF_BASE_PATH ?= ../../nrf5x-base
include $(NRF_BASE_PATH)/make/Makefile
