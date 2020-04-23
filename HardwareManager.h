#ifndef HARDWAREMANAGER_H
#define HARDWAREMANAGER_H
  #include <stdint.h>

  class HardwareManager {
    public:

      /**
       * Enables any hardware components
       */
      static void enableHardware();

      /**
       * Disables any hardware components
       */
      static void disableHardware();
  };
#endif
