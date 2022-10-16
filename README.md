# DSMR-tester
Arduino Uno firmware to emulate the P1 telegram output of DSMR-compatible digital meters.

## Usage
Instructions below are for use with the [P1-dongle](https://github.com/plan-d-io/P1-dongle).
1. Compile the code in the Arduino IDE and upload to the Uno at 115200bps
2. Connect the GND pins of the Arduino and the P1 dongle
3. Connect pin 3 of the Arduino to the Rx pin of the P1 dongle

Optionally, you can also connect the 5V pins of the Arduino and the dongle to provide power to the dongle. **Do not do this if you are powering the dongle through USB.** 
If you are powering the Arduino through USB, it's possible the Arduino will not be able to provide enough power for the dongle to work reliably. 

Every 2 seconds, the firmware will output the example meter telegram from the [DSMR 5.0.2 P1 Companion Standard](https://www.netbeheernederland.nl/_upload/Files/Slimme_meter_15_a727fce1f1.pdf). The values for actual power are randomised and the values for energy offtake continously increment, allowing end-to-end data pipeline verification. When the telegram is being sent, the Arduino LED will light up.
