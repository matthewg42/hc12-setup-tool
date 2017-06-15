#include <Arduino.h>
#include <MutilaDebug.h>
#include <Mode.h>
#include <SoftwareSerial.h>
#include <string.h>
#include "HC12Serial.h"
#include "Config.h"

// See Config.h for pins

bool channelPrinted = false;

const int CHANNEL_SCAN_TIME = 500;
const int SET_MODE_DELAY = 5;
const int BAUDRATES[] = {1200, 2400, 4800, 9600, 14400, 19200, 38400, 57600, 115200};

uint8_t channel = 0;
unsigned long lastChannelChange = 0;

HC12Serial hc12(HC12_RX, HC12_TX, HC12_SET);

void changeChannel()
{
    lastChannelChange = millis();
    hc12.setModeOn();
    delay(SET_MODE_DELAY);
    String cmd = "AT+C";
    channel = (channel+1) % 128;
    if (channel == 0) {
        Serial.println("Channel 0");
    }
    cmd += channel;
    DB("TX: ");
    DB(cmd);
    hc12.println(cmd);
    DB(" RX: ");
    DBLN(hc12.readStringUntil('\n'));
    hc12.setModeOff();
    channelPrinted = false;
}

void setHc12Baud(long baud)
{
    Serial.print("Setting baud to: ");
    Serial.println(baud);
    hc12.begin(baud);
    hc12.setTimeout(HC12_TIMEOUT_MS);
    hc12.setModeOn();
}

String getInput(String promptStr="> ") {
    Serial.print(promptStr);
    uint8_t i=0;
    char buf[64];
    memset(buf, 0, 64);
    while (true && i<64) {
        if (Serial.available() > 0) {
            int c = Serial.read();
            if (c<0) {
                Serial.println(F("serial error"));
                return "";
            }
            if (c == 127 && i>0) {
                // handle backspace
                i--;
                buf[i] = 0;
                Serial.print('\b');
                Serial.print(' ');
                Serial.print('\b');
            } else {
                if (c != '\b') { Serial.print((char)c); }
                if (c == '\r' || c == '\n') {
                    Serial.println();
                    return String(buf);
                }
                buf[i++] = (char)c;
            }
        }
    }
    Serial.println();
    return String(buf);
}

uint8_t getInt(uint8_t maximum, uint8_t def)
{
    while(true) {
        String p = "enter value 1-";
        p += maximum;
        p += " > ";
        p = getInput(p);
        if (p == "") {
            return def;
        }
        uint8_t i = p.toInt();
        if (i>maximum || i<1) {
            Serial.print("invalid: ");
            Serial.println(i);
        } else {
            return i;
        }
    }
}

void menuBaud()
{
    Serial.println(F("Select baud rate:"));
    uint8_t i;
    for (i=0; i<9; i++) {
        Serial.print(i+1);
        Serial.print(F(":   "));
        Serial.print(BAUDRATES[i]);
        if (i == 3) {
            Serial.println(" [default]");
        } else {
            Serial.println();
        }
    }
    i = getInt(9, 4)-1;
    setHc12Baud(BAUDRATES[i]);
}

void dumpSerial()
{
    hc12.setModeOff();
    hc12.flush();

    while (true) {
        if (Serial.available() > 0 || millis() > lastChannelChange + CHANNEL_SCAN_TIME) {
            break;
        }

        if (hc12.available() > 0 ) {
            int c = hc12.read();
            if (c < 0) {
                Serial.print("hc12 serial returned: ");
                Serial.print(c);
            }
            if (!channelPrinted) {
                Serial.print("\nCH");
                Serial.print(channel);
                Serial.print(": ");
                channelPrinted = true;
            }
            Serial.print((char)c);
        }
    }
    hc12.setModeOff();
}

void setup()
{
    Serial.begin(115200);
    Serial.setTimeout(1000000);
    delay(100);
    Serial.println(F("\n\n"));
    menuBaud();
}

void loop()
{
    dumpSerial();
    changeChannel();
}

