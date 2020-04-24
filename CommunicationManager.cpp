#include "CommunicationManager.h"

#include <Arduino.h>

#define Xbee Serial2

/*************************** STATIC ***************************/

/************************* Variables **************************/

CommunicationManager* CommunicationManager::instance = 0;


/************************** INSTANCE ***************************/

/********************** Private Functions **********************/

CommunicationManager::CommunicationManager() {
  Serial.println("Starting communication manager...");

  enableHardware();

  Serial.println("Communication manager successfully initialized");
}

void CommunicationManager::deviceDisabledCheck() {
  if(!isEnabled) {
    Serial.println("ERROR: CommunicationManager cannot perform action as it is disabled");
    exit(1868003);
  }
}

void CommunicationManager::transmitMeasurement(Measurement measurement) {
  
  // Check that device is enabled
  deviceDisabledCheck();

  // Get buffer
  byte* measurementBuffer = measurement.getBuffer();

  // Transmit data
  Xbee.readBytes(measurementBuffer, Measurement::MEASUREMENT_BUFFER_LENGTH);

  // Dealloc array
  delete [] measurementBuffer;
}

Measurement CommunicationManager::recieveMeasurement() {
  
  // Check that device is enabled
  deviceDisabledCheck();

  unsigned long startTime = millis();
  unsigned long endTime = startTime + (1000 * 30) ;
  while(startTime <= endTime || Xbee.available() > 0) {

    // Keep scanning if nothing is found
    if(Xbee.available() == 0) {
      continue;
    }

    // Create buffer to store data
    byte readBuffer[Measurement::MEASUREMENT_BUFFER_LENGTH];

    // Read available bytes up to the measurement length
    Xbee.readBytes(readBuffer, Measurement::MEASUREMENT_BUFFER_LENGTH);

    // Convert readBuffer into measurement
    return Measurement(&readBuffer, Measurement::MEASUREMENT_BUFFER_LENGTH);
  }
}


/********************** Public Functions **********************/

CommunicationManager* CommunicationManager::getInstance() {
  if(instance == 0) {
    instance = new CommunicationManager();
  }
  
  return instance;
}


void CommunicationManager::enableHardware() {

  // Start Xbee serial connection
  Xbee.begin(9600);
  while(!Xbee){}
  
  // Update hardware state as enabled
  ManagedHardware::enableHardware();
}

void CommunicationManager::disableHardware() {

  // Update hardware state as enabled
  ManagedHardware::disableHardware();

  // Close Xbee serial connection
  Xbee.end();
}

void CommunicationManager::performDataSync() {


}
