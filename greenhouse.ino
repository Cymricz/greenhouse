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
  int soilMoistureA;
  int soilMoistureB;
  
  // Returns true if 'soilCheckPeriod' has elapsed since last check
  if (sensorOne.moistureInterval(soilCheckPeriod, currentTime, previousTime))
  {
    digitalWrite(powerSwitch, HIGH); // give moisture sensors power
    soilMoistureA = sensorOne.readSoil();
    soilMoistureB = sensorTwo.readSoil();
    digitalWrite(powerSwitch, LOW); // kill power to moisture sensors
    previousTime = currentTime; // update 'previousTime' for next check
    
    if (soilMoistureA <= 50) // check if soil moisture value is at or below watering point
    {
      digitalWrite(dummyLight, LOW);
      digitalWrite(redLedA, HIGH);
    }
    if (soilMoistureB <= 50) // check if soil moisture value is at or below watering point
    {
      digitalWrite(dummyLight, LOW);
      digitalWrite(redLedB, HIGH);
    }
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
