#include "frog.h"

void Frog::setPins(uint8_t pinConnectJ, uint8_t pinConnectK) {
  this->pinConnectJ = pinConnectJ;
  this->pinConnectK = pinConnectK;

  // Make these pins mutually exclusive, i.e. only one can be active at a time.
  myMutex.setOutputPins(std::vector<uint8_t> {this->pinConnectJ, this->pinConnectK} );

  // // Add a delay to from all inactive to one pin active.
  // myMutex.setDelaymS(5000);

}

void Frog::setEvents(uint16_t eventIndexConnectJ, uint16_t eventIndexConnectK, uint16_t eventIndexDisconnect) {
  this->eventIndexConnectJ = eventIndexConnectJ;
  this->eventIndexConnectK = eventIndexConnectK;
  this->eventIndexDisconnect = eventIndexDisconnect;
}

void Frog::eventReceived(uint16_t receivedEventIndex) {
  // Compare receivedEventIndex with each of the three events for this frog.
  if (receivedEventIndex == this->eventIndexConnectJ) {
    myMutex.setPinActive(this->pinConnectJ);
  } else if (receivedEventIndex == this->eventIndexConnectK) {
    myMutex.setPinActive(this->pinConnectK);
  } else if (receivedEventIndex == this->eventIndexDisconnect) {
    myMutex.setAllPinsInActive();
  }
}

bool Frog::eventIndexMatchesThisFrog(uint16_t index) {
  if (index == this->eventIndexConnectJ) return true;
  if (index == this->eventIndexConnectK) return true;
  if (index == this->eventIndexDisconnect) return true;

  return false;
}
