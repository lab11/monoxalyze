PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
APPLICATION_SRCS += softdevice_handler.c
APPLICATION_SRCS += app_timer.c
APPLICATION_SRCS += led.c

SOFTDEVICE = $(HOME)/Programs/Nordic/softdevices/s110/s110_nrf51822_7.1.0_softdevice.hex

DEVICE = NRF51

USE_SOFTDEVICE = s110

SDK_PATH ?= $(HOME)/Programs/Nordic/sdk/nrf51822/
TEMPLATE_PATH ?= ld/

LIBRARY_PATHS += include
SOURCE_PATHS += src

CFLAGS = -Os
GDB_PORT_NUMBER = 2331

include $(TEMPLATE_PATH)Makefile
