// Sketch to run a small, semi-automated greenhouse
// Written by Cymric Cramer Sept 2017

#include <Time.h>
#include <TimeLib.h>

#include "SoilMoisture.h"
#include "DHT.h"
#include "Servo.h"

#define DHTPIN 4
#define DHTTYPE DHT11

SoilMoisture sensorOne(A0); // initialize moisture sensors
SoilMoisture sensorTwo(A1);
DHT dht(DHTPIN,DHTTYPE);    // initialize temp sensor
Servo alphaServo;
  
unsigned long previousTime{0};                  // two global variables for timing soil
const unsigned long soilCheckPeriod{43200000};  // moisture readings 
const int powerSwitch{2}; // set pin for transistor to sensors
const int redLedA{7};     // set pin for led indicator light on sensor A
const int redLedB{8};     // set pin for led indicator light on sensor B
const int dummyLight{12}; // set pin for green 'all-good' indicator led
const int redArray{3};
const int blueArray{5};
const int uvArray{6};
const int greenArray{9};

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(powerSwitch, OUTPUT); // controls transistor to switch power to moisture sensors
  digitalWrite(powerSwitch, LOW);
  pinMode(redLedA, OUTPUT);     // LED indicator if soil needs watering
  digitalWrite(redLedA, LOW);
  pinMode(redLedB, OUTPUT);
  digitalWrite(redLedB, LOW);
  pinMode(dummyLight, OUTPUT);
  digitalWrite(dummyLight, HIGH);
  pinMode(redArray, OUTPUT);
  digitalWrite(redArray, LOW);
  pinMode(blueArray, OUTPUT);
  digitalWrite(blueArray, LOW);
  pinMode(uvArray, OUTPUT);
  digitalWrite(uvArray, LOW);
  pinMode(greenArray, OUTPUT);
  digitalWrite(greenArray, LOW);
  // alphaServo.attach(9);
  
}

void loop() {
  delay(2000);
    
  setLedArrays();
  
  unsigned long currentTime{millis()};
  
  if (Serial.available())
  {
    char ch = Serial.read();
    if (ch = 1)
    {
      // force program to check moisture regardless of time
      goto moisture;
    }
    
  }
  
  // Returns true if 'soilCheckPeriod' has elapsed since last check
  if (sensorOne.moistureInterval(soilCheckPeriod, currentTime, previousTime))
  {
    moisture:
    digitalWrite(powerSwitch, HIGH); // send power to sensors    
    /*
     * Using a while loop turned out to be the best way I saw to check for low moisture and 
     * then keep the indicators on until the plant in question was watered. This method means
     * the sensors will continue to receive power until everything has been watered, which
     * is counter to my hope of keeping the sensor unpowered as much as possible to prevent
     * corrosion, but I couldn't see a better solution to the indicator LEDs.
     */
    int moistureA = sensorOne.readSoil();
    int moistureB = sensorTwo.readSoil();
        
    sensorOne.printReading(moistureA);
    sensorTwo.printReading(moistureB);

    // changed test to check for average of two sensors
    while ((sensorOne.readSoil() + sensorTwo.readSoil()) / 2 < 50 )
    {
      digitalWrite(dummyLight, LOW);
      digitalWrite(redLedA, HIGH);
      // digitalWrite(redLedB, HIGH);
    }
    /*
    while (sensorOne.readSoil() <= 30)
    {
      digitalWrite(dummyLight, LOW);
      digitalWrite(redLedA, HIGH);
    }
    while (sensorTwo.readSoil() <= 30)
    {
      digitalWrite(dummyLight, LOW);
      digitalWrite(redLedB, HIGH);
    }
    */
    digitalWrite(dummyLight, HIGH); // If we reach this line, then any sensor giving us a 
    digitalWrite(redLedA, LOW);     // low moisture reading has been watered, so we reset
    // digitalWrite(redLedB, LOW);  // all the indicator LEDs.
    
    digitalWrite(powerSwitch, LOW); // remove power from sensors
    previousTime = currentTime;     // update previousTime for the next check
    
  }
  
  float h = dht.readHumidity();
  float f = dht.readTemperature(true);
  // Check for failed read and exit early.
  if (isnan(h) || isnan(f))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
    
}

void setLedArrays()
{
  if (isAM())
  {
    digitalWrite(redArray, HIGH);
    digitalWrite(blueArray, HIGH);
    digitalWrite(uvArray, HIGH);
    digitalWrite(greenArray, HIGH);  
  }
  else if (isPM())
  {
    digitalWrite(redArray, LOW);
    digitalWrite(blueArray, LOW);
    digitalWrite(uvArray, LOW);
    digitalWrite(greenArray, LOW);
  }
  else
  {
    Serial.println("Time error");
  }
}

