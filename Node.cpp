#include "Node.h"

uint32_t Node::getUUID() {
  return *(uint32_t *)&FTFL_FCCOB7;
}
