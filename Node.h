#ifndef NODE_H
#define NODE_H
  #include <stdint.h>
  #include <Arduino.h>

  class Node {
    public:

      // Get the UUID for the current node
      static uint32_t getUUID();
  };
#endif
