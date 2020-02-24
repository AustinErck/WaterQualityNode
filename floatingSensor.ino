#include "Node.h"
#include "Measurement.h"
#include "DataManager.h"
#include "RtcManager.h"
#include "SensorManager.h"

void setup() {
  // (0) Initialize serial communication (DEBUG ONLY)
  Serial.begin(9600); // Set bits per second to 9600
  while(!Serial){};   // Wait for serial to start 
  Serial.println("Serial connection established!");

  // (1) WAKE UP HERE

  // (2) Init hardware
  DataManager* dataManager = DataManager::getInstance();
  RtcManager* rtcManager = RtcManager::getInstance();
  SensorManager* sensorManager = SensorManager::getInstance();

  // (4) Take/save measurement
  Measurement measurement = sensorManager->takeMeasurement();
  //dataManager->addMeasurement(measurement);

  /************************** NODE TEST **************************/

  dataManager->addNode(1234567890);

  uint32_t* nodes = dataManager->getNodes();

  uint8_t  nodeCount  = dataManager->getNodeCount();
  uint8_t  measurementCount  = dataManager->getMeasurementCount();

  Serial.print("Measurement count: ");
  Serial.println(measurementCount);

  dataManager->addNode(123498765);

  Serial.println("Node list:");
  for(int8_t i = 0; i < nodeCount; i++) {
    Serial.print(i);
    Serial.print(": ");
    Serial.println(*(nodes + i));
  }

  /************************** RTC TEST **************************/ 


  /*DS3232RTC Rtc(false);
  Rtc.begin();
  time_t myTime = Rtc.get();
  Serial.print("Time since epoch: ");
  Serial.println(myTime);*/
  
  Serial.println("Setup complete!");
}

void loop() {
  // put your main code here, to run repeatedly:
}
