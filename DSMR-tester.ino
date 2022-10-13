#include <SoftwareSerial.h>


/*Example DSMR telegram*/
const int ledPin = 13;
int rxPin = 2;
int txPin = 3;
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin, true);
float initEnergy = 123456.7;
float initInjection = 123.4;

void setup() {
  pinMode(ledPin, OUTPUT);
	Serial.begin(115200);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(115200);
}

void loop() {
  initEnergy = initEnergy + 0.01;
  initInjection = initInjection + 0.01; //random(0, 100)/10000.0;
  String power = "1-0:1.7.0(" + String(1.3 + random(0, 1000)/1000.0) + "*kW)";
  String energy = "1-0:1.8.1(" + String(initEnergy) + "*kWh)";
  String injection = "1-0:2.8.1(" + String(initInjection) + "*kWh)";
  digitalWrite(ledPin, HIGH);
  Serial.println("/ISk5\2MT382-1000");
  Serial.println("");
  Serial.println("1-3:0.2.8(50)");
  Serial.println("0-0:1.0.0(101209113020W)");
  Serial.println("0-0:96.1.1(4B384547303034303436333935353037)");
  //Serial.println("1-0:1.8.1(123456.789*kWh)");
  Serial.println(energy);
  Serial.println("1-0:1.8.2(123456.789*kWh)");
  //Serial.println("1-0:2.8.1(123456.789*kWh)");
  Serial.println(injection);
  Serial.println("1-0:2.8.2(123456.789*kWh)");
  Serial.println("0-0:96.14.0(0002)");
  //Serial.println("1-0:1.7.0(01.193*kW)");
  Serial.println(power);
  Serial.println("1-0:2.7.0(00.000*kW)");
  Serial.println("0-0:96.7.21(00004)");
  Serial.println("0-0:96.7.9(00002)");
  Serial.println("1-0:99.97.0(2)(0-0:96.7.19)(101208152415W)(0000000240*s)(101208151004W)(0000000301*s)");
  Serial.println("1-0:32.32.0(00002)");
  Serial.println("1-0:52.32.0(00001)");
  Serial.println("1-0:72.32.0(00000)");
  Serial.println("1-0:32.36.0(00000)");
  Serial.println("1-0:52.36.0(00003)");
  Serial.println("1-0:72.36.0(00000)");
  Serial.println("0-0:96.13.0(303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F)");
  Serial.println("1-0:32.7.0(220.1*V)");
  Serial.println("1-0:52.7.0(220.2*V)");
  Serial.println("1-0:72.7.0(220.3*V)");
  Serial.println("1-0:31.7.0(001*A)");
  Serial.println("1-0:51.7.0(002*A)");
  Serial.println("1-0:71.7.0(003*A)");
  Serial.println("1-0:21.7.0(01.111*kW)");
  Serial.println("1-0:41.7.0(02.222*kW)");
  Serial.println("1-0:61.7.0(03.333*kW)");
  Serial.println("1-0:22.7.0(04.444*kW)");
  Serial.println("1-0:42.7.0(05.555*kW)");
  Serial.println("1-0:62.7.0(06.666*kW)");
  Serial.println("0-1:24.1.0(003)");
  Serial.println("0-1:96.1.0(3232323241424344313233343536373839)");
  Serial.println("0-1:24.2.1(101209112500W)(12785.123*m3)");
  Serial.println("!EF2F");
  /*Now on the softwareserial*/
  mySerial.println("/ISk5\2MT382-1000");
  mySerial.println("");
  mySerial.println("1-3:0.2.8(50)");
  mySerial.println("0-0:1.0.0(101209113020W)");
  mySerial.println("0-0:96.1.1(4B384547303034303436333935353037)");
  mySerial.println(energy);
  mySerial.println("1-0:1.8.2(123456.789*kWh)");
  mySerial.println(injection);
  mySerial.println("1-0:2.8.2(123456.789*kWh)");
  mySerial.println("0-0:96.14.0(0002)");
  mySerial.println(power);
  mySerial.println("1-0:2.7.0(00.000*kW)");
  mySerial.println("0-0:96.7.21(00004)");
  mySerial.println("0-0:96.7.9(00002)");
  mySerial.println("1-0:99.97.0(2)(0-0:96.7.19)(101208152415W)(0000000240*s)(101208151004W)(0000000301*s)");
  mySerial.println("1-0:32.32.0(00002)");
  mySerial.println("1-0:52.32.0(00001)");
  mySerial.println("1-0:72.32.0(00000)");
  mySerial.println("1-0:32.36.0(00000)");
  mySerial.println("1-0:52.36.0(00003)");
  mySerial.println("1-0:72.36.0(00000)");
  mySerial.println("0-0:96.13.0(303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F)");
  mySerial.println("1-0:32.7.0(220.1*V)");
  mySerial.println("1-0:52.7.0(220.2*V)");
  mySerial.println("1-0:72.7.0(220.3*V)");
  mySerial.println("1-0:31.7.0(001*A)");
  mySerial.println("1-0:51.7.0(002*A)");
  mySerial.println("1-0:71.7.0(003*A)");
  mySerial.println("1-0:21.7.0(01.111*kW)");
  mySerial.println("1-0:41.7.0(02.222*kW)");
  mySerial.println("1-0:61.7.0(03.333*kW)");
  mySerial.println("1-0:22.7.0(04.444*kW)");
  mySerial.println("1-0:42.7.0(05.555*kW)");
  mySerial.println("1-0:62.7.0(06.666*kW)");
  mySerial.println("0-1:24.1.0(003)");
  mySerial.println("0-1:96.1.0(3232323241424344313233343536373839)");
  mySerial.println("0-1:24.2.3(101209112500W)(12785.123*m3)");
  mySerial.println("!EF2F");
  digitalWrite(ledPin, LOW);
  delay(2000);
}
