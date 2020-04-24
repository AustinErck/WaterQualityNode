#include "HardwareManager.h"
#include "SensorManager.h"
#include "DataManager.h"
#include "CommunicationManager.h"

#define POWER_PIN 16

/*************************** STATIC ***************************/

/********************** Public Functions **********************/

void HardwareManager::enableHardware() {

  // Configure power control pin
  pinMode(POWER_PIN, OUTPUT);

  // Enable power to sensors
  digitalWrite(POWER_PIN, HIGH);

  // Start sensorManager (includes hardware enable pin)
  SensorManager* sensorManager = SensorManager::getInstance();
  sensorManager->enableHardware();

  // Start dataManager
  DataManager* dataManager = DataManager::getInstance();
  dataManager->enableHardware();

  // Start communicationManager
  CommunicationManager* communicationManager = CommunicationManager::getInstance();
  communicationManager->enableHardware();
}

void HardwareManager::disableHardware() {

  // Stop dataManager
  DataManager* dataManager = DataManager::getInstance();
  dataManager->disableHardware();

  // Stop sensorManager (includes hardware enable pin)
  SensorManager* sensorManager = SensorManager::getInstance();
  sensorManager->disableHardware();

  // Stop communicationManager
  CommunicationManager* communicationManager = CommunicationManager::getInstance();
  communicationManager->disableHardware();

  // Disable power to sensors
  digitalWrite(POWER_PIN, LOW);
}
