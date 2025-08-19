#include "frog.h"

Frog::Frog(uint8_t pinConnectJ, uint8_t pinConnectK) {
  // Set the pins.
  this->pinConnectJ = pinConnectJ;
  this->pinConnectK = pinConnectK;

  // Make these pins mutually exclusive, i.e. only one can be active at a time.
  myMutex.setOutputPins(std::vector<uint8_t> {this->pinConnectJ, this->pinConnectK} );

  // Set the default active state.
  myMutex.setOutputsActiveLow();

  // Set both pins inactive.
  myMutex.setAllPinsInActive();

  // Set the initial state.
  this->currentState = State::DISCONNECTED;
}

void Frog::setEvents(uint16_t eventIndexConnectJ, uint16_t eventIndexDisconnectJ, uint16_t eventIndexConnectK, uint16_t eventIndexDisconnectK) {
  this->eventIndexConnectJ = eventIndexConnectJ;
  this->eventIndexDisconnectJ = eventIndexDisconnectJ;
  this->eventIndexConnectK = eventIndexConnectK;
  this->eventIndexDisconnectK = eventIndexDisconnectK;
}

void Frog::eventReceived(uint16_t receivedEventIndex) {
  // Compare receivedEventIndex with each of the four events for this frog.
  if (receivedEventIndex == this->eventIndexConnectJ) {
    myMutex.setPinActive(this->pinConnectJ);
    this->currentState = State::CONNECTED_J;
  } else if (receivedEventIndex == this->eventIndexConnectK) {
    myMutex.setPinActive(this->pinConnectK);
    this->currentState = State::CONNECTED_K;
  } else if (receivedEventIndex == this->eventIndexDisconnectJ) {
    myMutex.setAllPinsInActive();
    this->currentState = State::DISCONNECTED;
  } else if (receivedEventIndex == this->eventIndexDisconnectK) {
    myMutex.setAllPinsInActive();
    this->currentState = State::DISCONNECTED;
  }
}

// bool Frog::eventIndexMatchesThisFrog(uint16_t index) {
//   if ((index == this->eventIndexConnectJ) ||
//       (index == this->eventIndexConnectK) ||
//       (index == this->eventIndexDisconnect)) return true;

//   return false;
// }
bool Frog::eventIndexMatches(uint16_t index) {
  if ((index == this->eventIndexConnectJ) ||
      (index == this->eventIndexDisconnectJ) ||
      (index == this->eventIndexConnectK) ||
      (index == this->eventIndexDisconnectK)) return true;

  return false;
}

bool Frog::eventIndexMatchesCurrentState(uint16_t index) {
  if (index == this->eventIndexConnectJ) {
    return (currentState == State::CONNECTED_J);
  } 
  
  if (index == this->eventIndexConnectK) {
    return (currentState == State::CONNECTED_K);
  }

  if (index == this->eventIndexDisconnectJ) {
    return (currentState == State::DISCONNECTED);
  }

  if (index == this->eventIndexDisconnectK) {
    return (currentState == State::DISCONNECTED);
  }

  return false; // To keep the compiler happy!
}

void Frog::sendEventsForCurrentState(){}

void Frog::print() {
  //Serial.printf("\npinConnectJ=%d, pinConnectK=%d", pinConnectJ, pinConnectK);
  Serial.printf("\neventIndexConnectJ=%#02X, eventIndexDisconnectJ=%#02X, eventIndexConnectK=%#02X, eventIndexDisconnectK=%#02X,", eventIndexConnectJ, eventIndexDisconnectJ, eventIndexConnectK, eventIndexDisconnectK);
}
