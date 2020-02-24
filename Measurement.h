#ifndef MEASUREMENT_H
#define MEASUREMENT_H
  #include <stdint.h>

  #include "Node.h"
  #include "RtcManager.h"

  class Measurement {
    private:
      // UUID of the node that took the measurement
      uint32_t node;

      // Datetime the measurement was taken at
      uint32_t datetime;

      // Measured temperature
      uint16_t temp;

      // Measured salinity
      uint16_t salinity;

    public:

      /**
       * Create a new measurement
       */
      Measurement(uint16_t measuredTemp, uint16_t measuredSalinity);

      /**
       * Create a new instance of a measurement
       */
      Measurement(uint32_t nodeUUID, uint32_t date, uint16_t measuredTemp, uint16_t measuredSalinity);

      /**
       * Get measurement's device UUID
       */
      uint32_t getNodeUUID();

      /**
       * Get measurement's datetime
       */
      uint32_t getDateTime();

      /**
       * Get measurement's measured temperature
       */
      uint16_t getTemperature();

      /**
       * Get measurement's measured temperature
       */
      uint16_t getSalinity();
  };
#endif
