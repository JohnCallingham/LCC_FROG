#include "frog.h"

void FROG::setPins(uint8_t pinConnectJ, uint8_t pinConnectK) {
  this->pinConnectJ = pinConnectJ;
  this->pinConnectK = pinConnectK;

  // Make these pins mutually exclusive, i.e. only one can be active at a time.



}

void FROG::setEvents(uint16_t eventIndexConnectJ, uint16_t eventIndexConnectK, uint16_t eventIndexDisconnect) {
  this->eventIndexConnectJ = eventIndexConnectJ;
  this->eventIndexConnectK = eventIndexConnectK;
  this->eventIndexDisconnect = eventIndexDisconnect;
}

// "dependencies":
// {
//     "bblanchon/ArduinoJson": "^6.16.1",
//     "me-no-dev/AsyncTCP": "*",
//     "external-repo": "https://github.com/user/package.git#1.2.3",
//     "external-zip": "https://github.com/me-no-dev/AsyncTCP/archive/master.zip"
// }
