#include "HC12Serial.h"

HC12Serial::HC12Serial(uint8_t rxPin, uint8_t txPin, uint8_t setPin) : 
    SoftwareSerial(rxPin, txPin),
    _setPin(setPin)
{
}

void HC12Serial::begin(long baud)
{
    pinMode(_setPin, OUTPUT);
    setModeOff();
    SoftwareSerial::begin(baud);
}

void HC12Serial::setModeOff()
{
    digitalWrite(_setPin, HIGH);
}

void HC12Serial::setModeOn()
{
    digitalWrite(_setPin, LOW);
}

