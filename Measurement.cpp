#include "Measurement.h"

/********************** Public Functions **********************/

Measurement::Measurement(uint16_t measuredTemp, uint16_t measuredSalinity) {
  RtcManager* rtcManager = RtcManager::getInstance();
  node = Node::getUUID();
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
