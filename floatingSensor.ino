#include "Node.h"
#include "Measurement.h"
#include "HardwareManager.h"
#include "DataManager.h"
#include "RtcManager.h"
#include "SensorManager.h"

#define DEBUG
#define TESTING

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

  // Init hardware on start up (Formats storage, sets RTC if needed, checks connections to sensors, etc)
  HardwareManager::enableHardware();
  sensorManager = SensorManager::getInstance();
  dataManager = DataManager::getInstance();
  rtcManager = RtcManager::getInstance();
  Serial.println("Completed start up hardware initilization\n");

  // Run tests if testing mode is enabled
  #ifdef TESTING

    Serial.println("Beginning Storage Test\n");
  
    // Create measurement
    Measurement measurement = sensorManager->takeMeasurement();

    // Save measurement to FRAM
    uint16_t measurementIndex = dataManager->addMeasurement(measurement);

    // Print initial measurement data
    Serial.println("Initial measurement information:");
    Serial.print("Device: ");
    Serial.println(measurement.getNodeUUID());
    Serial.print("Datetime: ");
    Serial.println(measurement.getDateTime());
    Serial.print("Temp: ");
    Serial.println(measurement.getTemperature()/16.0); //Divided by 16.0 to get actual temperature in C
    Serial.print("Salinity: ");
    Serial.println(measurement.getSalinity());

    // Disables external hardware components
    Serial.println("\nDisabling hardware");
    HardwareManager::disableHardware();
    
    // Enables external hardware components
    Serial.println("Enabling hardware");
    HardwareManager::enableHardware();;

    // Create measurement
    Measurement readMeasurement = dataManager->getMeasurement(measurementIndex);

    // Print initial measurement data
    Serial.println("\nRead measurement information:");
    Serial.print("Device: ");
    Serial.println(readMeasurement.getNodeUUID());
    Serial.print("Datetime: ");
    Serial.println(readMeasurement.getDateTime());
    Serial.print("Temp: ");
    Serial.println(readMeasurement.getTemperature()/16.0); //Divided by 16.0 to get actual temperature in C
    Serial.print("Salinity: ");
    Serial.println(readMeasurement.getSalinity());
    
    //rtcManager->goToSleep();
  #endif
}

void loop() {

  /************************** Life cycle **************************/ 

  // (1) Init hardware
  //sensorManager->enableSensors();
  //Serial.println("Successfully woke hardware");
  
  delay(1000);
}
