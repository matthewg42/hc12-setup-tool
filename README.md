# HC12 Setup Tool

HC12 modules are 433Mhz band wireless serial modules. These modues are set up using a set of AT commands.  This project is a very hasily hacked together tool to make it easy to configure one of these little do-hickies.

## Instructions

* Wire your ESP8266 to your HC12 according to the pinout defined in the `Config.h` file
    * `HC12_RX` connects to the TX pin of the HC12
    * `HC12_TX` connects to the RX pin of the HC12
    * `HC12_SET` connects to the SET pin of the HC12 (pin 5)
* Also connect 3.3v power to the HC12/VIN and GND to GND
* Build and upload the sketch to your microcontroller
* Connect a serial terminal to your microcontroller, running at 115200 baud
    * If you use the Arduino IDE Serial Monitor, set line ending to newline.
* Reset the microcontroller 
* You'll should see the baud menu 
    * Choose the baud rate for your HC12 unit
    * The default is 9600 bps, which is how they come from the factory
* At the main command prompt, use `h` for help
* Type in commands (e.g. `AT+RX`), press return to execute command
* Reset your microcontroller to choose a different HC12 baud rate

## Note

This sketch uses SoftwareSerial for communications with the HC12. I recommend using a fastish microcontroller with this sketch (I use a NodeMCU / ESP8266). Older Arduinos like unos and nanos might be less reliable with the SoftwareSerial if the speed goes much above 9600. Might still work though.  :)

## Pre-requisites

* Install the Arduino IDE 1.6.6 or later
    * Windows & Mac users may find a download link [here](https://www.arduino.cc/en/Main/Software)
    * Major Linux distros carry it in their software repositories, or the link above may be used
* In the Arduino IDE, install ESP8266 board support from Tools -> Board -> Boards Manager
* In the Arduino IDE, install ESP8266 tools from Sketch -> Include Library -> Manage Libraries

## Building Using the Arduino IDE

This method is preferred on Windows and for casual users who just want to build and use the project.

* Open Arduino IDE 
* Open firmware/firmware.ino
* Set the board you are using (under the Tools -> Board menu)
* Connect board with USB cable 
* Click the Upload button on the tool bar

## Using make on Linux

This alternative build system may also be used. It is an alternative to using the Arduino IDE and may be preferred by some users. 

* Install the esp/Arduino [Makefile](https://github.com/plerup/makeEspArduino)
* Edit the `esp.mk` file and set the include line to point at where you installed `makeEspArduino.mk` (or set the `ESP_MAKEFILE` environment variable)
* Edit `firmware/Makefile` and uncomment a BOARD for the board you're using (to see a list of possible boards, run `make list_boards` from the firmware directory)
* Connect your ESP board with a USB cable
* Press and hold the flash button on the board and use the command `make flash` from the firmware directory

