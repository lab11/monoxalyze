PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
APPLICATION_SRCS += softdevice_handler.c
APPLICATION_SRCS += ble_debug_assert_handler.c
APPLICATION_SRCS += ble_advdata.c
APPLICATION_SRCS += bleApp.c
APPLICATION_SRCS += app_timer.c
APPLICATION_SRCS += app_scheduler.c
APPLICATION_SRCS += led.c
APPLICATION_SRCS += ledAuth.c
APPLICATION_SRCS += lps25h.c
APPLICATION_SRCS += si7021.c
APPLICATION_SRCS += battery.c
APPLICATION_SRCS += lmp91000.c
APPLICATION_SRCS += twi_hw_master.c
APPLICATION_SRCS += adc.c
APPLICATION_SRCS += ble_conn_params.c
APPLICATION_SRCS += app_error.c
APPLICATION_SRCS += app_util_platform.c
APPLICATION_SRCS += ble_co.c
#APPLICATION_SRCS += ble_auth.c
APPLICATION_SRCS += ble_srv_common.c
APPLICATION_SRCS += queue.c



SOFTDEVICE = $(HOME)/Programs/Nordic/softdevices/s110/s110_nrf51822_7.1.0_softdevice.hex

DEVICE = NRF51
BOARD = MONOXALYZE

USE_SOFTDEVICE = s110

SDK_VERSION = 7

#if SDK_VERSION = 6, point SDK_PATH to sdk/model/
#if SDK_VERSION = 7, point SDK_PATH to sdk/

ifeq ($(SDK_VERSION), 7)
	SDK_PATH ?= $(HOME)/Programs/Nordic/sdk/
else
	SDK_PATH ?= $(HOME)/Programs/Nordic/sdk/nrf51822/
endif

TEMPLATE_PATH ?= ../../nrf51-pure-gcc-setup/template/


LIBRARY_PATHS += ../../drivers/include
LIBRARY_PATHS += ../../services
LIBRARY_PATHS += include
SOURCE_PATHS += ../../drivers/src
SOURCE_PATHS += ../../services
SOURCE_PATHS += src

CFLAGS += -Os
GDB_PORT_NUMBER = 2331

include $(TEMPLATE_PATH)Makefile
