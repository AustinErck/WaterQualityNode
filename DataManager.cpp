#include "DataManager.h"
#include "Node.h"
#include "Measurement.h"

#include <Arduino.h>
#include "Adafruit_FRAM_I2C.h"

#define FRAM_I2C_ADDRESS        80

// Document constants
#define MAX_NODES               24
#define MAX_MEASUREMENTS        4352

#define DEVICE_DATA_BASE                0
#define DEVICE_DATA_SIZE                80 * 2
#define DEVICE_NODE_COUNT_OFFSET        5
#define DEVICE_MEASUREMENT_COUNT_OFFSET 7 

#define NODES_BASE    DEVICE_DATA_BASE + DEVICE_DATA_SIZE
#define NODE_SIZE     274 * 2

#define MEASUREMENTS_BASE   NODES_BASE + (NODE_SIZE * MAX_NODES)
#define MEASUREMENT_SIZE    6

Adafruit_FRAM_I2C Fram; //Rename FRAM class name to FRAM

/*************************** STATIC ***************************/

/************************* Variables **************************/

DataManager* DataManager::instance = 0;


/************************** INSTANCE ***************************/

/********************** Private Functions **********************/

DataManager::DataManager() {
  Serial.println("Starting data manager...");

  // Check connection to FRAM
  if(!Fram.begin(FRAM_I2C_ADDRESS)) {
    Serial.println("Unable to initialize FRAM");
    while (!Fram.begin(FRAM_I2C_ADDRESS)) delay(10000); // Keep trying to initialize FRAM chip every 10 seconds
  }
  Serial.println("FRAM successfully initialized");

  // Format FRAM if needed
  if(!isFramFormatted()){
    formatFram();
  }

  Serial.print("Device UUID: ");
  Serial.println(Node::getUUID());
}

uint32_t DataManager::read32(uint16_t baseAddress) {
  uint32_t value = 0;
  
  for(uint8_t i = 0; i < 4; i++) {

    // Read the value from Fram
    uint8_t readValue = Fram.read8(baseAddress + i);

    // Combine values from little endian format
    value += readValue << (8*i);
  }

  return value;
}

void DataManager::write32(uint16_t baseAddress, uint32_t value) {
  
  // For each byte of the 32 bit value
  for(uint8_t i = 0; i < 4; i++) {
    
    // Determine which byte to write
    uint8_t writeValue = value & 0xFF;

    // Write byte
    Fram.write8(baseAddress + i, writeValue);

    // Shift out written values
    value = value >> 8;
  }
}

void DataManager::write16(uint16_t baseAddress, uint16_t value) {

  Fram.write8(baseAddress, value & 0xFF);
  Fram.write8(baseAddress, (value >> 8) & 0xFF);
}

bool DataManager::isFramFormatted() {

  // Read the first two words of FRAM
  uint32_t deviceUUID = read32(0);

  // return false;
  return Node::getUUID() == deviceUUID;
}

void DataManager::formatFram() {
  Serial.println("Formatting FRAM...");
  
  // Write device UUID to the first two words(4 bytes)
  write32(DEVICE_DATA_BASE, Node::getUUID());

  // Set the node count to initial value of 1
  setNodeCount(0);

  // Set measurement count to initial value of 0
  setMeasurementCount(0);

  // Add current node as first node
  addNode(Node::getUUID());
}

void DataManager::setNodeCount(uint8_t nodeCount) {
  
  // Write node count to FRAM
  Fram.write8(DEVICE_DATA_BASE + DEVICE_NODE_COUNT_OFFSET, nodeCount);
}

void DataManager::setMeasurementCount(uint16_t measurementCount) {
  
  // Write measurement count to correct bytes
  Fram.write8(DEVICE_DATA_BASE + DEVICE_MEASUREMENT_COUNT_OFFSET, measurementCount & 0xFF);
  Fram.write8(DEVICE_DATA_BASE + DEVICE_MEASUREMENT_COUNT_OFFSET + 1, measurementCount >> 8 & 0xFF);
}


/********************** Public Functions **********************/

DataManager* DataManager::getInstance() {
  if(instance == 0) {
    instance = new DataManager();
  }
  
  return instance;
}

uint8_t DataManager::getNodeCount() {

  uint8_t temp = Fram.read8(DEVICE_DATA_BASE + DEVICE_NODE_COUNT_OFFSET);
  Serial.print("getNodeCount: ");
  Serial.println(temp);
  
  // Read node count to FRAM
  return temp;
}

uint16_t DataManager::getMeasurementCount() {
  uint16_t measurementCount = Fram.read8(DEVICE_DATA_BASE + DEVICE_MEASUREMENT_COUNT_OFFSET);

  // Add second byte to measurement count
  measurementCount += Fram.read8(DEVICE_DATA_BASE + DEVICE_MEASUREMENT_COUNT_OFFSET + 1) << 8;

  return measurementCount;
}

uint32_t* DataManager::getNodes() {

  uint8_t nodeCount = getNodeCount();

  // Create a new array
  uint32_t* nodes = new uint32_t[nodeCount];

  // Set address initial value
  uint16_t address = NODES_BASE;

  for(uint16_t i = 0; i < nodeCount; i++) {
    // Increment address to next node
    address += NODE_SIZE;

    // Read node UUID and add to node array
    *(nodes + i) = read32(address);
  }

  return nodes;
}

uint8_t DataManager::addNode(uint32_t nodeUUID) {
  Serial.println("Adding node...");

  // Get current node count
  uint8_t nodeCount = getNodeCount();

  // Check that max nodes has not been reached
  if(nodeCount >= MAX_NODES) {
    Serial.println("ERROR: Unable to add node. At max value already");
    return 255;
  }

  // Grab all currently added nodes
  uint32_t* nodes = getNodes();

  // Determine if node has been added before and exit if it has been added
  for(int8_t i = 0; i < nodeCount; i++) {
    if( *(nodes + i) == nodeUUID) {
      return i;
    }
  }

  // Update current node count in FRAM
  setNodeCount(nodeCount + 1);

  // Set address to the node base address for the next node
  uint16_t address = NODES_BASE + nodeCount * NODE_SIZE;

  // Writes nodeUUID to FRAM
  write32(address, nodeUUID);

  // Increment address by 4 to skip to the measurement propagation bits
  address += 4;

  // Clears document propagation bytes
  for(uint16_t i = 0; i < NODE_SIZE-4; i++) {
    Fram.write8(address + i, 0);
  }

  Serial.println("Node added.");
  return nodeCount;
}

uint16_t DataManager::addMeasurement(Measurement measurement) {
  Serial.println("Adding measurement...");

  // Get current measurement count and add one
  uint16_t measurementCount = getMeasurementCount();

  // Check that max nodes has not been reached
  if(measurementCount >= MAX_MEASUREMENTS) {
    Serial.println("ERROR: Unable to add measurement. At max value already");
    return 65535;
  }

  // Update current node count in FRAM
  setMeasurementCount(measurementCount + 1);

  // Set address to the node base address for the next node
  uint16_t address = MEASUREMENTS_BASE + measurementCount * MEASUREMENT_SIZE;

  // Writes nodeUUID, datetime, temp, and salinity to FRAM
  write32(address, measurement.getNodeUUID());
  write32(address + 4, measurement.getDateTime());
  write16(address + 8, measurement.getTemperature());
  write16(address + 10, measurement.getSalinity());

  Serial.println("Measurement added.");
  return measurementCount;
}
