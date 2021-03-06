#ifndef NODE_H
#define NODE_H
  #include <stdint.h>
  #include <Arduino.h>

  class Node {
    public:

      // WARNING: For this project we were allowed to make the assumption that nodes know every other node in the network
      // List of node uuids (also determines the order in which nodes transmit)
      static uint32_t const DEVICE_UUIDS[];
      static uint8_t const DEVICE_COUNT;
      static uint32_t const DEVICE_UUID = 576850;

      // Get the UUID for the current node
      //static uint32_t getUUID();
  };
#endif
