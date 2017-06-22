# Either define ARDUINO_MAKEFILE in the environment to be the full path 
# to where you have Arduino.mk installed, or just edit it here...
ifndef ARDUINO_MAKEFILE
	ARDUINO_MAKEFILE := /opt/Arduino-Makefile/Arduino.mk
endif

include $(ARDUINO_MAKEFILE)


