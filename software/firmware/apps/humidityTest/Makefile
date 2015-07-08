PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
APPLICATION_SRCS += app_timer.c
APPLICATION_SRCS += app_scheduler.c
APPLICATION_SRCS += si7021.c
APPLICATION_SRCS += twi_hw_master.c
APPLICATION_SRCS += app_error.c
APPLICATION_SRCS += app_util_platform.c

SOFTDEVICE = $(HOME)/Programs/Nordic/softdevices/s110/s110_nrf51822_7.1.0_softdevice.hex

DEVICE = NRF51
BOARD = MONOXALYZE

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
LIBRARY_PATHS += include
LIBRARY_PATHS += ../../services
LIBRARY_PATHS += .
SOURCE_PATHS += ../../drivers/src
SOURCE_PATHS += src
SOURCE_PATHS += ../../services
SOURCE_PATHS += ../../src

CFLAGS += -Os
GDB_PORT_NUMBER = 2331

all: all

collect: collect.jlink
	JLinkExe collect.jlink > collect.out

collect.jlink:
	printf "r\nmem32 0 0x4000\nmem32 0x00010000 0x4000\nq" > collect.jlink

include $(TEMPLATE_PATH)Makefile
