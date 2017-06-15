#pragma once

#include <Arduino.h>
#include <stdint.h>
#include <SoftwareSerial.h>

class HC12Serial : public SoftwareSerial {
public:
    HC12Serial(uint8_t rxPin, uint8_t txPin, uint8_t setPin);
    void begin(long baud);
    void setModeOn();
    void setModeOff();

protected:
    uint8_t _setPin;

};

