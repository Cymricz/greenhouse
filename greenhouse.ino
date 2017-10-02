// Sketch to run a small, semi-automated greenhouse
// Written by Cymric Cramer Sept 2017

#include "SoilMoisture.h"
#include "DHT.h"
#include "Servo.h"

#define DHTPIN 4
#define DHTTYPE DHT11

SoilMoisture sensorOne(A0); // initialize moisture sensors
SoilMoisture sensorTwo(A1);
DHT dht(DHTPIN,DHTTYPE);    // initialize temp sensor
Servo alphaServo;
  
unsigned long previousTime{0}; // two global variables for timing soil
const unsigned long soilCheckPeriod{86400000};  // moisture readings 
const int powerSwitch{2}; // set pin for transistor to sensors
const int redLedA{7}; // set pin for led indicator light on sensor A
const int redLedB{8}; // set pin for led indicator light on sensor B
const int dummyLight{12}; // set pin for green 'all-good' indicator led

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(powerSwitch, OUTPUT); // controls transistor to switch power to moisture sensors
  digitalWrite(powerSwitch, LOW);
  pinMode(redLedA, OUTPUT); // LED indicator if soil needs watering
  digitalWrite(redLedA, LOW);
  pinMode(redLedB, OUTPUT);
  digitalWrite(redLedB, LOW);
  pinMode(dummyLight, HIGH);
  alphaServo.attach(9);
  
}

void loop() {
  delay(2000);

  unsigned long currentTime{millis()};
  
  // Returns true if 'soilCheckPeriod' has elapsed since last check
  if (sensorOne.moistureInterval(soilCheckPeriod, currentTime, previousTime))
  {
    digitalWrite(powerSwitch, HIGH); // send power to sensors
    int x;
    if (sensorOne.readSoil() <= 50 && sensorTwo.readSoil() <= 50)
    {
      digitalWrite(dummyLight, LOW);
      digitalWrite(redLedA, HIGH);
      digitalWrite(redLedB, HIGH);
    }
    else if (sensorOne.readSoil() <= 50)
    {
      digitalWrite(dummyLight, LOW);
      digitalWrite(redLedA, HIGH);
    }
    else if (sensorTwo.readSoil() <= 50)
    {
      digitalWrite(dummyLight, LOW);
      digitalWrite(redLedB, HIGH);
    }
    digitalWrite(powerSwitch, LOW); // remove power from sensors
    currentTime = previousTime; // reset currentTime for the next check
  }
  
  float h = dht.readHumidity();
  float f = dht.readTemperature(true);
  // Check for failed read and exit early.
  if (isnan(h) || isnan(f))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  bool lidOpen = true;
 
  
  

}
