#include "Measurement.h"

/********************** Public Functions **********************/

Measurement::Measurement(uint16_t measuredTemp, uint16_t measuredSalinity) {
  RtcManager* rtcManager = RtcManager::getInstance();
  node = Node::DEVICE_UUID;
  datetime = rtcManager->getDatetime();
  temp = measuredTemp;
  salinity = measuredSalinity;
}

Measurement::Measurement(uint32_t nodeUUID, uint32_t date, uint16_t measuredTemp, uint16_t measuredSalinity) {  
  node = nodeUUID;
  datetime = date;
  temp = measuredTemp;
  salinity = measuredSalinity;
}

Measurement::Measurement(byte* measurementArray, uint8_t arrayLength) { 
   
   //Check if arrayLength is correct
   if(arrayLength != Measurement::MEASUREMENT_BUFFER_LENGTH) {
    Serial.println("Fatal Error: Invalid mesurement array size");
    exit(1868004);
   }

  // Update local variables
  node = (measurementArray[3] << 24) + (measurementArray[2] << 16) + (measurementArray[1] << 8) + measurementArray[0];
  datetime = (measurementArray[7] << 24) + (measurementArray[6] << 16) + (measurementArray[5] << 8) + measurementArray[4];
  temp = (measurementArray[9] << 8) + measurementArray[8];
  salinity = (measurementArray[11] << 8) + measurementArray[10];

  // Determine expectedSum
  uint16_t expectedSum = (measurementArray[17] << 8) + measurementArray[16];

  uint16_t sum = 0;
  for(uint8_t i = 0; i < arrayLength-2; i++) {
    sum += measurementArray[i];
  }

  if(expectedSum != sum) {
    Serial.println("Fatal Error: Measurement is invalid");
    exit(1868005);
  }
}

uint32_t Measurement::getNodeUUID() {
  return node;
}

uint32_t Measurement::getDateTime() {
  return datetime;
}

uint16_t Measurement::getTemperature() {
  return temp;
}

uint16_t Measurement::getSalinity() {
  return salinity;
}

byte* Measurement::getBuffer(uint32_t currentNodeUUID) {

  // Create buffer with measurement data
  byte* measurementBuffer = new byte[Measurement::MEASUREMENT_BUFFER_LENGTH];
  measurementBuffer[0] = node & 0xFF;
  measurementBuffer[1] = (node >> 8)  & 0xFF;
  measurementBuffer[2] = (node >> 16) & 0xFF;
  measurementBuffer[3] = (node >> 24) & 0xFF;
  measurementBuffer[4] = datetime & 0xFF;
  measurementBuffer[5] = (datetime >> 8)  & 0xFF;
  measurementBuffer[6] = (datetime >> 16) & 0xFF;
  measurementBuffer[7] = (datetime >> 24) & 0xFF;
  measurementBuffer[8] = temp & 0xFF;
  measurementBuffer[9] = (temp >> 8) & 0xFF;
  measurementBuffer[10] = salinity & 0xFF;
  measurementBuffer[11] = (salinity >> 8) & 0xFF;
  measurementBuffer[12] = currentNodeUUID & 0xFF;
  measurementBuffer[13] = (currentNodeUUID >> 8)  & 0xFF;
  measurementBuffer[14] = (currentNodeUUID >> 16)  & 0xFF;
  measurementBuffer[15] = (currentNodeUUID >> 24)  & 0xFF;

  // Calculate sum of all measurement data
  uint16_t sum = 0;
  for(int sumIndex = 0; sum < 12; sum++) {
    sum += measurementBuffer[sumIndex];
  }

  // Add sum to buffer
  measurementBuffer[16] = sum & 0xFF;
  measurementBuffer[17] = (sum >> 8) & 0xFF;
  
  return measurementBuffer;
}
