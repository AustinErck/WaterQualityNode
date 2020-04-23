#ifndef DATAMANAGER_H
#define DATAMANAGER_H
  #include <stdint.h>

  #include "ManagedHardware.h"
  #include "Measurement.h"

  class DataManager: public ManagedHardware {
    private:
      /********************** STATIC **********************/

      static DataManager* instance;   // Singleton instance of DataManager

      /** 
       *  Get's the device's UUID 
       *  @return Device's UUID
      */
      static uint32_t getDeviceUUID();
  
      /********************** INSTANCE **********************/

      /** 
       *  Create an instance of DataManager
      */
      DataManager();

      /**
       * Reads 32 bits of data from FRAM
       * @param baseAddress: Base address to begin read at. Uses addresses baseAddress through (baseAddress + 4)
      */
      uint32_t read32(uint16_t baseAddress);

      /**
       * Reads 16 bits of data from FRAM
       * @param baseAddress: Base address to begin read at. Uses addresses baseAddress through (baseAddress + 2)
      */
      uint16_t read16(uint16_t baseAddress);

      /**
       * Writes 32 bits of data to FRAM
       * @param baseAddress: Base address to begin write at. Uses addresses baseAddress through (baseAddress + 4)
       * @param value: Value being written to FRAM
      */
      void write32(uint16_t baseAddress, uint32_t value);

      /**
       * Writes 16 bits of data to FRAM
       * @param baseAddress: Base address to begin write at. Uses addresses baseAddress through (baseAddress + 2)
       * @param value: Value being written to FRAM
      */
      void write16(uint16_t baseAddress, uint16_t value);
  
      /** 
       *  Check that FRAM is formatted correctly
       *  @return If the FRAM is formatted correctly
      */
      bool isFramFormatted();
      
      /**
       * Format Fram
      */
      void formatFram();
  
      /**
       * Write node count to FRAM
      */
      void setNodeCount(uint8_t nodeCount);

      /**
       * Write measurement count to FRAM
       * @param measurementCount: Value setting measurement count
      */
      void setMeasurementCount(uint16_t measurementCount);
  
    public:
    
      /**
       * Gets instance of data manager
       */
      static DataManager* getInstance();

      /**
       * Enables hardware
      */
      void enableHardware();

      /**
       * Read node count from FRAM
      */
      uint8_t getNodeCount();

      /**
       * Read measurement count from FRAM
      */
      uint16_t getMeasurementCount();

      /**
       * Gets a pointer to a list of nodes
       */
      uint32_t* getNodes();

      /**
       * Add node to FRAM
       * @param nodeUUID: UUID of the node
      */
      uint8_t addNode(uint32_t nodeUUID);

      /**
       * Add measurement to FRAM
       * @param measurement: Measurement to save
      */
      uint16_t addMeasurement(Measurement measurement);

      /**
       * Get measurement from FRAM
       * @param measurementIndex: Index of measurement
      */
      Measurement getMeasurement(uint16_t measurementIndex);
  };
#endif
