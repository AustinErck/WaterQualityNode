#include "CommunicationManager.h"
#include "DataManager.h"

#include <Arduino.h>

// Transmit constants
#define ROUNDS            2                                   // A round is <node count> * 2 * <transmit window> + <transmit buffer> ms (includes both transmit and ack sections)
#define TRANSMIT_WINDOW   28 * 1000                           // ms a single node has to transmit data in
#define TRANSMIT_BUFFER   2 * 1000                            // ms between transmit windows
#define TRANSMIT_BLOCK    TRANSMIT_WINDOW + TRANSMIT_BUFFER   // A block is the transmit window and following buffer.

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
  byte* measurementBuffer = measurement.getBuffer(Node::DEVICE_UUID);

  // Transmit data
  Xbee.readBytes(measurementBuffer, Measurement::MEASUREMENT_BUFFER_LENGTH);

  // Dealloc array
  delete [] measurementBuffer;
}

Measurement* CommunicationManager::recieveMeasurement() {
  
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
    uint8_t bufferLength = Measurement::MEASUREMENT_BUFFER_LENGTH;
    Measurement* measurement = new Measurement((unsigned char *)readBuffer, bufferLength);
    
    return measurement;
  }

  return nullptr;
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

  // Determine which index the node is in node list
  uint8_t nodeTransmitIndex = 0;
  for(uint8_t index = 0; index < Node::DEVICE_COUNT; index++) {
    if(Node::DEVICE_UUIDS[index] == Node::DEVICE_UUID) {
      nodeTransmitIndex = index;
    }
  }

  // Determine start and end offset
  uint16_t startOffset = nodeTransmitIndex * TRANSMIT_BLOCK;
  uint16_t endOffset = startOffset + TRANSMIT_WINDOW;

  // Get DataManager instance
  DataManager* dataManager = DataManager::getInstance();

  // Get list of measurements to transmit
  uint16_t measurementCount = dataManager->getMeasurementCount();
  for(int16_t measurementIndex = 0; measurementIndex < measurementCount; measurementIndex++) {
    
  }

  // Track measurements recieved and which prop bits to update

  //While in sync frame
  //If in transmit window
  //  transmit measurement one at a time with gap in between
  //If NOT in transmit window
  //  Listen for measurements
  //  If new add to FRAM
  //  Then update prop bits new or not

  // Update prop bits
  // If all prop bits active, freeze data
}
