#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H
  #include <stdint.h>
  #include <Adafruit_MMA8451.h>

  #include "Measurement.h"

  class SensorManager {
    private:
      /********************** STATIC **********************/

      static SensorManager* instance;  // Singleton instance of SensorManager
     
  
      /********************** INSTANCE **********************/

      Adafruit_MMA8451 accelerometer; // Accelerometer instance

      bool isEnabled; // Determines if the sensors are currently powered on and initilized
      
      /** 
       *  Create an instance of SensorManager
      */
      SensorManager();

      /**
       * Start acceleromete
       */
      void initAccelerometer();

    public:
    
      /**
       * Gets instance of sensor manager
       */
      static SensorManager* getInstance();

      /**
       * Checks if the node is moving enough to not burn out the motor
       */
      bool isMoving();

      /**
       * Gets a temperature measurement
       */
      uint16_t measureTemperature();

       /**
       * Gets a salinity measurement
       */
      uint16_t measureSalinity();

      /**
       * Take a new measurement
       */
      Measurement takeMeasurement();
  };
#endif
