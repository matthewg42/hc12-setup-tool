#include <Arduino.h>
#include <MutilaDebug.h>
#include <Mode.h>
#include <SoftwareSerial.h>
#include <string.h>
#include <stdint.h>
#include "HC12Serial.h"
#include "Config.h"

// See Config.h for pins

bool help = false;

const long BAUDRATES[] = {1200, 2400, 4800, 9600, 14400, 19200, 38400, 57600, 115200};

HC12Serial hc12(HC12_RX, HC12_TX, HC12_SET);

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
    bool rxLast = false;

    Serial.println(F("CTRL-C to abort"));

    while (true) {
        if (Serial.available() > 0) {
            int c = Serial.read();
            if (c < 0 || c == 3) {
                break;
            } else {
                if (rxLast) {
                    rxLast = false;
                    Serial.print("\nTX: ");
                }
                Serial.print((char)c);
                hc12.write((char)c);
                if (c == '\r') {
                    Serial.print('\n');
                    hc12.write('\n');
                }
            }
        }

        if (hc12.available() > 0 ) {
            int c = hc12.read();
            if (c < 0) {
                Serial.print(F("HC12 serial returned: "));
                Serial.print(c);
            }
            if (!rxLast) {
                rxLast = true;
                Serial.print("\nRX: ");
            }
            Serial.print((char)c);
        }
    }
    hc12.setModeOff();
}

void sendATCommand()
{
    if (help) {
        Serial.println(F("AT          return OK to show comms working [default command]"));
        Serial.println();
        Serial.println(F("AT+Rx       query settings; x: C=channel F=mode P=power X=all"));
        Serial.println();
        Serial.println(F("AT+Bxxxx    set baud rate for device to xxxx, e.g. AT+B9600"));
        Serial.println();
        Serial.println(F("AT+Cxxx     set channel; 001 to 127"));
        Serial.println(F("            note: if channel > 100 you may have range issues"));
        Serial.println(F("            note: leave gaps of 5 channels to prevent crosstalk"));
        Serial.println();
        Serial.println(F("AT+FUx      set mode; x: 1=moderate powersave 2=powersave 3=fullspeed"));
        Serial.println(F("AT+Px       set xmit power; x: 1=min 8=max"));
        Serial.println(F("AT+Uxyz     set data bits (x), check bit (y), stop bit (z), e.g. AT+U8O1"));
        Serial.println(F("            x: 7 or 8"));
        Serial.println(F("            y: N=none, O=odd, E=even"));
        Serial.println(F("            z: 1=1, 2=2, 3=1.5"));
        Serial.println();
        Serial.println(F("AT+DEFAULT  reset"));
        Serial.println();
        Serial.println(F("AT+SLEEP    go to sleep"));
        Serial.println();
        Serial.println(F("c           apply changes now"));
        Serial.println(F("d           dump serial (press return to quit from there)"));
        Serial.println(F("h           re-display help message"));
        help = false;
    }

    String cmd = getInput("command (h for help) > ");
    if (cmd == "") {
        cmd = "AT";
    } else if (cmd[0] == 'd') {
        dumpSerial();
        return;
    } else if (cmd[0] == 'h') {
        help = true;
        return;
    } else if (cmd[0] == 'c') {
        hc12.setModeOff();
        hc12.setModeOn();
        return;
    }
    hc12.println(cmd);
    Serial.println(hc12.readString());
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
    sendATCommand();
}

