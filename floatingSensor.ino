#include "Node.h"
#include "Measurement.h"
#include "DataManager.h"
#include "RtcManager.h"
#include "SensorManager.h"

#define DEBUG 1

// Hardware managers
DataManager* dataManager;
RtcManager* rtcManager;
SensorManager* sensorManager;

void setup() {

  #ifdef DEBUG
  // (0) Initialize serial communication (DEBUG ONLY)
  Serial.begin(9600); // Set bits per second to 9600
  while(!Serial){};   // Wait for serial to start 
  Serial.println("Serial connection established!");
  #endif

  // (1) Init hardware on start up (Formats storage, sets RTC if needed, checks connections to sensors, etc)
  dataManager = DataManager::getInstance();
  rtcManager = RtcManager::getInstance();
  sensorManager = SensorManager::getInstance();
  Serial.println("Completed start up hardware initilization\n");

  // (3) Take/save measurement
  /*Measurement measurement = sensorManager->takeMeasurement();
  //dataManager->addMeasurement(measurement);
  Serial.print("Device: ");
  Serial.println(measurement.getNodeUUID());
  Serial.print("Datetime: ");
  Serial.println(measurement.getDateTime());
  Serial.print("Temp: ");
  Serial.println(measurement.getTemperature()/16.0);
  Serial.print("Salinity: ");
  Serial.println(measurement.getSalinity());*/

  // (8) Go to sleep
  //rtcManager->goToSleep();

  /************************** NODE TEST **************************/

  /*dataManager->addNode(1234567890);

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
  }*/

  /************************** RTC TEST **************************/ 


  /*DS3232RTC Rtc(false);
  Rtc.begin();
  time_t myTime = Rtc.get();
  Serial.print("Time since epoch: ");
  Serial.println(myTime);*/
}

void loop() {

  /************************** Life cycle **************************/ 

  // (1) Init hardware
  sensorManager->enableSensors();
  Serial.println("Successfully woke hardware");
  
  delay(1000);
}
