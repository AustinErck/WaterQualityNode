#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H
  #include <stdint.h>
  #include <Adafruit_MMA8451.h>
  #include <OneWire.h>

  #include "Measurement.h"

  class SensorManager {
    private:
      /********************** STATIC **********************/

      static SensorManager* instance;  // Singleton instance of SensorManager
     
  
      /********************** INSTANCE **********************/

      Adafruit_MMA8451 accelerometer = 0; // Accelerometer instance
      OneWire thermometer = 0;            // Thermometer instance
      byte thermometerAddress[8];

      bool isEnabled; // Determines if the sensors are currently powered on and initilized
      
      /** 
       *  Create an instance of SensorManager
      */
      SensorManager();

      /**
       * Start accelerometer
       */
      void initAccelerometer();

      /**
       * Start thermometer
       */
      void initThermometer();

    public:
    
      /**
       * Gets instance of sensor manager
       */
      static SensorManager* getInstance();

      /**
       * Enables the power to the sensors and initilizes any required hardware
       */
      void enableSensors();

      /**
       * Disables the power to the sensors
       */
      void disableSensors();

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
