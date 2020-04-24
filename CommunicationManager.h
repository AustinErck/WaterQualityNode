#ifndef COMMUNICATIONMANAGER_H
#define COMMUNICATIONMANAGER_H
  #include <stdint.h>

  #include "ManagedHardware.h"
  #include "Measurement.h"

  class CommunicationManager: public ManagedHardware {
    private:
      /********************** STATIC **********************/

      static CommunicationManager* instance;   // Singleton instance of DataManager
  
      /********************** INSTANCE **********************/

      /** 
       *  Create an instance of DataManager
      */
      CommunicationManager();   

      /**
       * Handles show stopper errors
       */
      void deviceDisabledCheck();

      /**
       * Write measurement to serial
       * @param measurement: Measurement to send
      */
      void transmitMeasurement(Measurement measurement);

      /**
       * Read measurement from serial
       * @return measurement: Measurement recieved
      */
      Measurement recieveMeasurement();
  
    public:
    
      /**
       * Gets instance of data manager
       */
      static CommunicationManager* getInstance();

      /**
       * Enables hardware
      */
      void enableHardware();

      /**
       * Disables hardware
      */
      void disableHardware();

      /**
       * Perform data sync with other nodes
       */
      void performDataSync();
  };
#endif
