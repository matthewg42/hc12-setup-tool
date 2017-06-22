#pragma once

#include <Arduino.h>

#ifdef ESP_H
// ESP8266 pinout
#define HC12_SET        D1
#define HC12_TX         D2
#define HC12_RX         D3

#else
// Arduino pinout
#define HC12_SET        5
#define HC12_TX         6
#define HC12_RX         7

#endif

#define HC12_TIMEOUT_MS 200
#define DEFAULT_BAUD    9600

