#include "SensorManager.h"

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

#define TEMP_PIN      14
#define SALINITY_PIN  15
#define ACCEL_ADDRESS 28

/*************************** STATIC ***************************/

/************************* Variables **************************/

SensorManager* SensorManager::instance = 0;

/************************** INSTANCE ***************************/

/************************* Variables **************************/

bool isEnabled = true;

/********************** Private Functions **********************/

SensorManager::SensorManager() {
  Serial.println("Starting sensor manager...");

  initAccelerometer();

  Serial.println("Sensor manager successfully initialized");
}

void SensorManager::initAccelerometer() {

  // Create new instance of accelerometer
  accelerometer = Adafruit_MMA8451();
  
  // Check connection to accelerometer
  if(!accelerometer.begin(ACCEL_ADDRESS)) {
    Serial.println("Unable to initialize accelerometer");
    while (!accelerometer.begin(ACCEL_ADDRESS)) delay(10000); // Keep trying to initialize accelerometer chip every 10 seconds
  }
  accelerometer.setRange(MMA8451_RANGE_2_G);
  Serial.println("Accelerometer successfully initialized");
}

/********************** Public Functions **********************/

SensorManager* SensorManager::getInstance() {
  if(instance == 0) {
    instance = new SensorManager();
  }
  
  return instance;
}

bool SensorManager::isMoving() {
  // Check if sensors are enabled
  if(!isEnabled) {
    return false;
  }

  /* Get a new sensor event */ 
  sensors_event_t event; 
  accelerometer.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: \t"); Serial.print(event.acceleration.x); Serial.print("\t");
  Serial.print("Y: \t"); Serial.print(event.acceleration.y); Serial.print("\t");
  Serial.print("Z: \t"); Serial.print(event.acceleration.z); Serial.print("\t");
  Serial.println("m/s^2 ");

  // Determine when we consider the node moving here...

  return false;
}

uint16_t SensorManager::measureTemperature() {
  // Check if sensors are enabled
  if(!isEnabled) {
    return 65535;
  }

  // Calibration code goes here
  
  uint16_t analogVoltageValue = analogRead(TEMP_PIN);
  return analogVoltageValue;
}

uint16_t SensorManager::measureSalinity() {
  // Check if sensors are enabled
  if(!isEnabled) {
    return 65535;
  }
  
  uint16_t analogVoltageValue = analogRead(TEMP_PIN);

  // Calibration code goes here
  
  return analogVoltageValue;
}

Measurement SensorManager::takeMeasurement() {
  uint32_t temp = measureTemperature();
  uint16_t salinity = measureSalinity();
  
  return Measurement(temp, salinity);
}
