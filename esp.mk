# Either define ESP_MAKEFILE in the environment to be the full path 
# to where you have makeEspArduino.mk installed, or just edit it here...
ifndef ESP_MAKEFILE
	ESP_MAKEFILE := /opt/makeEspArduino/makeEspArduino.mk
endif

ifeq ($(CHIP), esp32)
	ARDUINO_ROOT := $(shell grep -o "sketchbook.path=.*" $(HOME)/.arduino/preferences.txt 2>/dev/null | cut -f2- -d=)
	ARDUINO_LIBS := $(ARDUINO_ROOT)/libraries
	ESP_ROOT     := $(ARDUINO_ROOT)/hardware/espressif/esp32
endif

include $(ESP_MAKEFILE)


