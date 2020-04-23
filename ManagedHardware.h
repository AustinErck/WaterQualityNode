#ifndef MANAGEDHARDWARE_H
#define MANAGEDHARDWARE_H
  #include <stdint.h>

  class ManagedHardware {
    protected:
      
      bool isEnabled = false;

    public:

      /**
       * Enables any hardware components
       */
      void enableHardware();

      /**
       * Disables any hardware components
       */
      void disableHardware();
  };
#endif
