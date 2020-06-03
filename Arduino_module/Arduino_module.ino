#include "DHT.h"
#include "Wire.h"
#include "LPS.h"

DHT dht;
LPS ps;

const int DHTPin = A0;
const int rainfallPin = A1;
const int insolationPin = A2;
// A5 & A4 (SDA & SCL) -> pressure and temprature
const int ledPin = 3;

float temperatureDHTValue = 0;
float humidityDHTValue = 0;
float temperatureLPSValue = 0;
float pressureLPSValue = 0;
float rainfallValue = 0;
float insolationValue = 0;

float rainfall = 0;
float insolation = 0;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  Wire.begin();
  dht.setup(DHTPin);
  
  pinMode(3, OUTPUT); 

  if (!ps.init())
  {
    Serial.println("Failed to autodetect pressure sensor!");
    while (1);
  }
  ps.enableDefault();
}

void loop() {
  // DHT11:
  temperatureDHTValue = dht.getTemperature();
  humidityDHTValue = dht.getHumidity();
  
  // LPS
  temperatureLPSValue = ps.readTemperatureC();
  pressureLPSValue = ps.readPressureMillibars();

  // Rainfall
  rainfallValue = analogRead(rainfallPin);
  rainfall = map(rainfallValue, 1025, 0, 0, 100);
  
  // Insolation
  insolationValue = analogRead(insolationPin);
  insolation = map(insolationValue, 0, 1025, 0 , 100);
  
//-------------------------------------------------------------------------------------------------------------------------
// Send data frame:

Serial.print("$_");
Serial.print(temperatureLPSValue);
Serial.print("_");
Serial.print(humidityDHTValue);
Serial.print("_");
Serial.print(pressureLPSValue);
Serial.print("_");
Serial.print(rainfall);
Serial.print("_");
Serial.print(insolation);
Serial.println("_X");



/*
// Show value on serial port
  Serial.println("----------------------------------------");
  Serial.print("Temperature(DHT): ");
  Serial.print(temperatureDHTValue);
  Serial.print(" C \t");
  Serial.print("Humidity(DHT): ");
  Serial.print(humidityDHTValue);
  Serial.println("% \t");

  Serial.print("Temperature(LPS): ");
  Serial.print(temperatureLPSValue);
  Serial.print(" C \t");
  Serial.print("Pressure(LPS): ");
  Serial.print(pressureLPSValue);
  Serial.println("mbar \t");

  Serial.print("Rainfall: ");
  Serial.println(rainfallValue);

  Serial.print("Insolation: ");
  Serial.println(insolationValue);



*/

  delay(2000);
}
