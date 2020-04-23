#include "ManagedHardware.h"

/************************** INSTANCE ***************************/

/********************** Public Functions **********************/

void ManagedHardware::enableHardware() {
  isEnabled = true;
}

void ManagedHardware::disableHardware() {
  isEnabled = false;
}
