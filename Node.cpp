#include "Node.h"

uint32_t Node::DEVICE_UUIDS[] = { 576850, 576851, 576852 };

uint32_t Node::getUUID() {
  return *(uint32_t *)&FTFL_FCCOB7;
}
