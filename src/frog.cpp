#include "frog.h"

void FROG::setPins(uint8_t pinConnectJ, uint8_t pinConnectK) {
  this->pinConnectJ = pinConnectJ;
  this->pinConnectK = pinConnectK;

  // Make these pins mutually exclusive, i.e. only one can be active at a time.
  myMutex.setOutputPins(std::vector<uint8_t> {this->pinConnectJ, this->pinConnectK} );

  // Add a delay to from all inactive to one pin active.
  myMutex.setDelaymS(5000);

}

void FROG::setEvents(uint16_t eventIndexConnectJ, uint16_t eventIndexConnectK, uint16_t eventIndexDisconnect) {
  this->eventIndexConnectJ = eventIndexConnectJ;
  this->eventIndexConnectK = eventIndexConnectK;
  this->eventIndexDisconnect = eventIndexDisconnect;
}

void FROG::process() {
  myMutex.process();
}

void FROG::eventReceived(uint16_t receivedEventIndex) {
  // Compare receivedEventIndex with each of the three events for this frog.
  if (receivedEventIndex == this->eventIndexConnectJ) {
    // Set pinConnectJ active.
    myMutex.setPinActive(this->pinConnectJ);
  } else if (receivedEventIndex == this->eventIndexConnectK) {
    // Set pinConnectK active.
    myMutex.setPinActive(this->pinConnectK);
  } else if (receivedEventIndex == this->eventIndexDisconnect) {
    // Make pinConnectJ and pinConnectK both inactive.
    myMutex.setAllPinsInActive();
  }
}
