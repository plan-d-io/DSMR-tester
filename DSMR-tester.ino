#include <SoftwareSerial.h>
#include <TimeLib.h>
#include <elapsedMillis.h>

#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 
const unsigned long DEFAULT_TIME = 1704284920; // Jan 3 2024

const int ledPin = 13;
int rxPin = 2;
int txPin = 3;
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin, true);
float initEnergy = 123456.7;
float initInjection = 123.4;
float totGasCon = 321.12;
bool gasUpdated = false;
unsigned long gasTime, ppTime;
float highPP;
bool ppUpdated = false;
unsigned long meterTime;
bool quarterHour = true;
elapsedMillis waitForTime;

String floatToStringFormatted(float value, bool shortFormat = false) {
  //Serial.println(value);
  // Ensure the value is positive
  if (value < 0) value = 0;
  // Prepare variables for integer and decimal parts
  unsigned long intValue;
  unsigned int decimalValue;
  // Handling large values by reducing them to fit within the expected format
  if (value >= 1000000) {
    intValue = 999999; // Max value to fit in format
    decimalValue = 999;
  }
  else {
    intValue = static_cast<unsigned long>(value); // Integer part of the float
    decimalValue = static_cast<unsigned int>((value - intValue) * 1000); // Decimal part
  }
  char buffer[12]; // Buffer size for the string
  // Format the string based on the shortFormat flag
  if (shortFormat) {
    snprintf(buffer, sizeof(buffer), "%02lu.%03u", intValue % 100, decimalValue); // Short format
  }
  else {
    snprintf(buffer, sizeof(buffer), "%06lu.%03u", intValue, decimalValue); // Long format
  }
  // Debugging print of the buffer content
  //for (int i = 0; i < sizeof(buffer); i++) Serial.print(buffer[i]);
  //Serial.println(); // New line for clarity
  return String(buffer);
}


bool checkQuarterHour() {
  int m = minute();
  return m == 0 || m == 15 || m == 30 || m == 45;
}

String formatDateTime(unsigned long epochTime) {
  tmElements_t tm;
  breakTime(epochTime, tm);
  String yearStr = String(tm.Year + 1970).substring(2); // Get last two digits of the year
  String monthStr = tm.Month < 10 ? "0" + String(tm.Month) : String(tm.Month);
  String dayStr = tm.Day < 10 ? "0" + String(tm.Day) : String(tm.Day);
  String hourStr = tm.Hour < 10 ? "0" + String(tm.Hour) : String(tm.Hour);
  String minuteStr = tm.Minute < 10 ? "0" + String(tm.Minute) : String(tm.Minute);
  String secondStr = tm.Second < 10 ? "0" + String(tm.Second) : String(tm.Second);
  return yearStr + monthStr + dayStr + hourStr + minuteStr + secondStr + "W";
}

void setup() {
  pinMode(ledPin, OUTPUT);
	Serial.begin(115200);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(115200);
  setSyncProvider( requestSync);  //set function to call when sync required
  Serial.println("Waiting for sync message");
}

void loop() {
<<<<<<< HEAD
  if(Serial.available()){
    processSyncMessage();
  }
  if(timeStatus() == timeNotSet && waitForTime > 60000){
    Serial.println("Time-out, using default time");
    setTime(DEFAULT_TIME);
  }
  if(timeStatus()!= timeNotSet){
    meterTime = now();
    initEnergy = initEnergy + 0.01;
    initInjection = initInjection + 0.01; //random(0, 100)/10000.0;
    if(quarterHour){
      time_t currentTime = now();
      if(!gasUpdated){
        totGasCon += 1.0;
        gasTime = currentTime - 60;
        gasUpdated = true;
        quarterHour = false;
      }
      if(!ppUpdated){
        highPP = 1.3 + random(0, 1000)/1000.0;
        ppTime = currentTime;
        ppUpdated = true;
        quarterHour = false;
      }
    }
    else{
      quarterHour = checkQuarterHour();
      gasUpdated = false;
      ppUpdated = false;
    }
    digitalWrite(ledPin, HIGH);
    Serial.println(F("/FLU5\253769484_A0"));
    Serial.println("");
    Serial.println(F("0-0:96.1.4(50217)")); //Belgian P1 version information
    Serial.println(F("1-3:0.2.8(50)")); //Generic DSMR version information
    Serial.println("0-0:1.0.0(" + formatDateTime(meterTime) +")"); //Meter timestamp
    Serial.println(F("0-0:96.1.1(4B384547303034303436333935353037)")); //Equipment ID
    Serial.println("1-0:1.8.1(" + floatToStringFormatted(initEnergy, false) + "*kWh)"); //Consumed energy tariff 1
    Serial.println(F("1-0:1.8.2(123456.789*kWh)")); //Consumed energy tariff 2
    Serial.println("1-0:2.8.1(" + floatToStringFormatted(initInjection, false) + "*kWh)"); //Injected energy tariff 1
    Serial.println(F("1-0:2.8.2(123456.789*kWh)")); //Injected energy tariff 2
    Serial.println(F("0-0:96.14.0(0002)")); //Active tariff period
    Serial.println("1-0:1.7.0(" + floatToStringFormatted((1.3 + random(0, 1000)/1000.0), true) + "*kW)"); //Consumed active power
    Serial.println(F("1-0:2.7.0(00.000*kW)")); //Injected active power
    Serial.println(F("1-0:1.4.0(02.351*kW)")); //Current average demand
    Serial.println("1-0:1.6.0(" + formatDateTime(ppTime) + ")(02.589*kW)"); //Maximum demand of the running month
    Serial.println(F("0-0:98.1.0(3)(1-0:1.6.0)(1-0:1.6.0)(200501000000S)(200423192538S)(03.695*kW)(200401000000S)(200305122139S)(05.980*kW)(200301000000S)(200210035421W)(04.318*kW)")); //Maximum demand of the last 13 months
    Serial.println(F("0-0:96.13.0(303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F)")); //Text message
    Serial.println(F("1-0:32.7.0(220.1*V)")); //Instantaneous voltage L1
    Serial.println(F("1-0:52.7.0(220.2*V)")); //Instantaneous voltage L2
    Serial.println(F("1-0:72.7.0(220.3*V)")); //Instantaneous voltage L3
    Serial.println(F("1-0:31.7.0(001.11*A)")); //Instantaneous current L1
    Serial.println(F("1-0:51.7.0(002.22*A)")); //Instantaneous current L2
    Serial.println(F("1-0:71.7.0(003.33*A)")); //Instantaneous current L3
    Serial.println(F("1-0:21.7.0(01.111*kW)")); //Instantaneous active power consumption L1
    Serial.println(F("1-0:41.7.0(02.222*kW)")); //Instantaneous active power consumption L2
    Serial.println(F("1-0:61.7.0(03.333*kW)")); //Instantaneous active power consumption L3
    Serial.println(F("1-0:22.7.0(04.444*kW)")); //Instantaneous active power injection L1
    Serial.println(F("1-0:42.7.0(05.555*kW)")); //Instantaneous active power injection L2
    Serial.println(F("1-0:62.7.0(06.666*kW)")); //Instantaneous active power injection L3
    Serial.println(F("0-1:24.1.0(003)")); //Mbus channel 1 device type (003 = gas meter)
    Serial.println(F("0-1:96.1.0(3232323241424344313233343536373839)")); //Mbus channel 1 device ID
    Serial.println(F("0-1:96.1.1(37464C4F32313139303333373333)")); //Mbus channel 1 device ID according DIN 43863-5
    Serial.println("0-1:24.2.3("+ formatDateTime(gasTime) + ")(" + floatToStringFormatted(totGasCon, false) + "*m3)"); //Last 5-minute non-temperature compensated gas meter value
    Serial.println(F("0-2:24.1.0(007)")); //Mbus channel 1 device type (007 = water meter)
    Serial.println(F("0-1:96.1.0(3232323241424344313233343536373839)")); //Mbus channel 1 device ID
    Serial.println(F("0-1:96.1.1(37464C4F32313139303333373333)")); //Mbus channel 2 device ID according DIN 43863-5
    Serial.println(F("0-2:24.2.1(2312316558S)(00112.384*m3)")); //Last 5-minute water meter value
    Serial.println(F("!XXX"));
    /*Now on the softwareserial*/
    mySerial.println(F("/FLU5\253769484_A0"));
    mySerial.println("");
    mySerial.println(F("0-0:96.1.4(50217)")); //Belgian P1 version information
    mySerial.println(F("1-3:0.2.8(50)")); //Generic DSMR version information
    mySerial.println("0-0:1.0.0(" + formatDateTime(meterTime) +")"); //Meter timestamp
    mySerial.println(F("0-0:96.1.1(4B384547303034303436333935353037)")); //Equipment ID
    mySerial.println("1-0:1.8.1(" + floatToStringFormatted(initEnergy, false) + "*kWh)"); //Consumed energy tariff 1
    mySerial.println(F("1-0:1.8.2(123456.789*kWh)")); //Consumed energy tariff 2
    mySerial.println("1-0:2.8.1(" + floatToStringFormatted(initInjection, false) + "*kWh)"); //Injected energy tariff 1
    mySerial.println(F("1-0:2.8.2(123456.789*kWh)")); //Injected energy tariff 2
    mySerial.println(F("0-0:96.14.0(0002)")); //Active tariff period
    mySerial.println("1-0:1.7.0(" + floatToStringFormatted((1.3 + random(0, 1000)/1000.0), true) + "*kW)"); //Consumed active power
    mySerial.println(F("1-0:2.7.0(00.000*kW)")); //Injected active power
    mySerial.println(F("1-0:1.4.0(02.351*kW)")); //Current average demand
    mySerial.println("1-0:1.6.0(" + formatDateTime(ppTime) + ")(02.589*kW)"); //Maximum demand of the running month
    mySerial.println(F("0-0:98.1.0(3)(1-0:1.6.0)(1-0:1.6.0)(200501000000S)(200423192538S)(03.695*kW)(200401000000S)(200305122139S)(05.980*kW)(200301000000S)(200210035421W)(04.318*kW)")); //Maximum demand of the last 13 months
    mySerial.println(F("0-0:96.13.0(303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F)")); //Text message
    mySerial.println(F("1-0:32.7.0(220.1*V)")); //Instantaneous voltage L1
    mySerial.println(F("1-0:52.7.0(220.2*V)")); //Instantaneous voltage L2
    mySerial.println(F("1-0:72.7.0(220.3*V)")); //Instantaneous voltage L3
    mySerial.println(F("1-0:31.7.0(001.11*A)")); //Instantaneous current L1
    mySerial.println(F("1-0:51.7.0(002.22*A)")); //Instantaneous current L2
    mySerial.println(F("1-0:71.7.0(003.33*A)")); //Instantaneous current L3
    mySerial.println(F("1-0:21.7.0(01.111*kW)")); //Instantaneous active power consumption L1
    mySerial.println(F("1-0:41.7.0(02.222*kW)")); //Instantaneous active power consumption L2
    mySerial.println(F("1-0:61.7.0(03.333*kW)")); //Instantaneous active power consumption L3
    mySerial.println(F("1-0:22.7.0(04.444*kW)")); //Instantaneous active power injection L1
    mySerial.println(F("1-0:42.7.0(05.555*kW)")); //Instantaneous active power injection L2
    mySerial.println(F("1-0:62.7.0(06.666*kW)")); //Instantaneous active power injection L3
    mySerial.println(F("0-1:24.1.0(003)")); //Mbus channel 1 device type (003 = gas meter)
    mySerial.println(F("0-1:96.1.0(3232323241424344313233343536373839)")); //Mbus channel 1 device ID
    mySerial.println(F("0-1:96.1.1(37464C4F32313139303333373333)")); //Mbus channel 1 device ID according DIN 43863-5
    mySerial.println("0-1:24.2.3("+ formatDateTime(gasTime) + ")(" + floatToStringFormatted(totGasCon, false) + "*m3)"); //Last 5-minute non-temperature compensated gas meter value
    mySerial.println(F("0-2:24.1.0(007)")); //Mbus channel 1 device type (007 = water meter)
    mySerial.println(F("0-1:96.1.0(3232323241424344313233343536373839)")); //Mbus channel 1 device ID
    mySerial.println(F("0-1:96.1.1(37464C4F32313139303333373333)")); //Mbus channel 2 device ID according DIN 43863-5
    mySerial.println(F("0-2:24.2.1(2312316558S)(00112.384*m3)")); //Last 5-minute water meter value
    mySerial.println(F("!XXX"));
    digitalWrite(ledPin, LOW);
    delay(2000);
  }
}

void processSyncMessage() {
  unsigned long pctime;
  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
     }
  }
}

time_t requestSync(){
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
=======
  initEnergy = initEnergy + 0.01;
  initInjection = initInjection + 0.01; //random(0, 100)/10000.0;
  String power = "1-0:1.7.0(" + String(1.3 + random(0, 1000)/1000.0) + "*kW)";
  String energy = "1-0:1.8.1(" + String(initEnergy) + "*kWh)";
  String injection = "1-0:2.8.1(" + String(initInjection) + "*kWh)";
  digitalWrite(ledPin, HIGH);
  Serial.println("/FLU5\253769484_A0");
  Serial.println("");
  Serial.println("0-0:96.1.4(50217)"); //Belgian P1 version information
  Serial.println("1-3:0.2.8(50)"); //Generic DSMR version information
  Serial.println("0-0:1.0.0(101209113020W)"); //Meter timestamp
  Serial.println("0-0:96.1.1(4B384547303034303436333935353037)"); //Equipment ID
  Serial.println(energy); //Consumed energy tariff 1
  Serial.println("1-0:1.8.2(123456.789*kWh)"); //Consumed energy tariff 2
  Serial.println(injection); //Injected energy tariff 1
  Serial.println("1-0:2.8.2(123456.789*kWh)"); //Injected energy tariff 2
  Serial.println("0-0:96.14.0(0002)"); //Active tariff period
  Serial.println(power); //Consumed active power
  Serial.println("1-0:2.7.0(00.000*kW)"); //Injected active power
  Serial.println("1-0:1.4.0((02.351*kW)"); //Current average demand
  Serial.println("1-0:1.6.0(200509134558S)(02.589*kW)"); //Maximum demand of the running month
  Serial.println("0-0:98.1.0(3)(1-0:1.6.0)(1-0:1.6.0)(200501000000S)(200423192538S)(03.695*kW)(200401000000S)(200305122139S)(05.980*kW)(200301000000S)(200210035421W)(04.318*kW)"); //Maximum demand of the last 13 months
  Serial.println("0-0:96.13.0(303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F)"); //Text message
  Serial.println("1-0:32.7.0(220.1*V)"); //Instantaneous voltage L1
  Serial.println("1-0:52.7.0(220.2*V)"); //Instantaneous voltage L2
  Serial.println("1-0:72.7.0(220.3*V)"); //Instantaneous voltage L3
  Serial.println("1-0:31.7.0(001*A)"); //Instantaneous current L1
  Serial.println("1-0:51.7.0(002*A)"); //Instantaneous current L2
  Serial.println("1-0:71.7.0(003*A)"); //Instantaneous current L3
  Serial.println("1-0:21.7.0(01.111*kW)"); //Instantaneous active power consumption L1
  Serial.println("1-0:41.7.0(02.222*kW)"); //Instantaneous active power consumption L2
  Serial.println("1-0:61.7.0(03.333*kW)"); //Instantaneous active power consumption L3
  Serial.println("1-0:22.7.0(04.444*kW)"); //Instantaneous active power injection L1
  Serial.println("1-0:42.7.0(05.555*kW)"); //Instantaneous active power injection L2
  Serial.println("1-0:62.7.0(06.666*kW)"); //Instantaneous active power injection L3
  Serial.println("0-1:24.1.0(003)"); //Mbus channel 1 device type (003 = gas meter)
  Serial.println("0-1:96.1.0(3232323241424344313233343536373839)"); //Mbus channel 1 device ID
  Serial.println("0-1:96.1.1(37464C4F32313139303333373333)"); //Mbus channel 1 device ID according DIN 43863-5
  //Serial.println("0-1:24.2.1(101209112500W)(12785.123*m3)"); //Last 5-minute water meter value
  Serial.println("0-1:24.2.3(200512134558S)(00112.384*m3"); //Last 5-minute non-temperature compensated gas meter value
  Serial.println("0-2:24.1.0(007)"); //Mbus channel 1 device type (007 = water meter)
  Serial.println("0-1:96.1.0(3232323241424344313233343536373839)"); //Mbus channel 1 device ID
  Serial.println("0-1:96.1.1(37464C4F32313139303333373333)"); //Mbus channel 2 device ID according DIN 43863-5
  Serial.println("0-2:24.2.1(200512134558S)(00112.384*m3"); //Last 5-minute water meter value
  Serial.println("!XXX");
  /*Now on the softwareserial*/
  mySerial.println("/FLU5\253769484_A0");
  mySerial.println("");
  mySerial.println("0-0:96.1.4(50217)"); //Belgian P1 version information
  mySerial.println("1-3:0.2.8(50)"); //Generic DSMR version information
  mySerial.println("0-0:1.0.0(101209113020W)"); //Meter timestamp
  mySerial.println("0-0:96.1.1(4B384547303034303436333935353037)"); //Equipment ID
  mySerial.println(energy); //Consumed energy tariff 1
  mySerial.println("1-0:1.8.2(123456.789*kWh)"); //Consumed energy tariff 2
  mySerial.println(injection); //Injected energy tariff 1
  mySerial.println("1-0:2.8.2(123456.789*kWh)"); //Injected energy tariff 2
  mySerial.println("0-0:96.14.0(0002)"); //Active tariff period
  mySerial.println(power); //Consumed active power
  mySerial.println("1-0:2.7.0(00.000*kW)"); //Injected active power
  mySerial.println("1-0:1.4.0((02.351*kW)"); //Current average demand
  mySerial.println("1-0:1.6.0(200509134558S)(02.589*kW)"); //Maximum demand of the running month
  mySerial.println("0-0:98.1.0(3)(1-0:1.6.0)(1-0:1.6.0)(200501000000S)(200423192538S)(03.695*kW)(200401000000S)(200305122139S)(05.980*kW)(200301000000S)(200210035421W)(04.318*kW)"); //Maximum demand of the last 13 months
  mySerial.println("0-0:96.13.0(303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F)"); //Text message
  mySerial.println("1-0:32.7.0(220.1*V)"); //Instantaneous voltage L1
  mySerial.println("1-0:52.7.0(220.2*V)"); //Instantaneous voltage L2
  mySerial.println("1-0:72.7.0(220.3*V)"); //Instantaneous voltage L3
  mySerial.println("1-0:31.7.0(001*A)"); //Instantaneous current L1
  mySerial.println("1-0:51.7.0(002*A)"); //Instantaneous current L2
  mySerial.println("1-0:71.7.0(003*A)"); //Instantaneous current L3
  mySerial.println("1-0:21.7.0(01.111*kW)"); //Instantaneous active power consumption L1
  mySerial.println("1-0:41.7.0(02.222*kW)"); //Instantaneous active power consumption L2
  mySerial.println("1-0:61.7.0(03.333*kW)"); //Instantaneous active power consumption L3
  mySerial.println("1-0:22.7.0(04.444*kW)"); //Instantaneous active power injection L1
  mySerial.println("1-0:42.7.0(05.555*kW)"); //Instantaneous active power injection L2
  mySerial.println("1-0:62.7.0(06.666*kW)"); //Instantaneous active power injection L3
  mySerial.println("0-1:24.1.0(003)"); //Mbus channel 1 device type (003 = gas meter)
  mySerial.println("0-1:96.1.0(3232323241424344313233343536373839)"); //Mbus channel 1 device ID
  mySerial.println("0-1:96.1.1(37464C4F32313139303333373333)"); //Mbus channel 1 device ID according DIN 43863-5
  //mySerial.println("0-1:24.2.1(101209112500W)(12785.123*m3)"); //Last 5-minute water meter value
  mySerial.println("0-1:24.2.3(200512134558S)(00112.384*m3"); //Last 5-minute non-temperature compensated gas meter value
  mySerial.println("0-2:24.1.0(007)"); //Mbus channel 1 device type (007 = water meter)
  mySerial.println("0-1:96.1.0(3232323241424344313233343536373839)"); //Mbus channel 1 device ID
  mySerial.println("0-1:96.1.1(37464C4F32313139303333373333)"); //Mbus channel 2 device ID according DIN 43863-5
  mySerial.println("0-2:24.2.1(200512134558S)(00112.384*m3"); //Last 5-minute water meter value
  mySerial.println("!XXX");
  digitalWrite(ledPin, LOW);
  delay(2000);
>>>>>>> 7eb57857442baf90f70c4903d0dcdea19cdca546
}
