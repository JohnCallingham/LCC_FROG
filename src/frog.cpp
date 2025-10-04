#include "frog.h"

Frog::Frog(uint8_t frogNumber, uint8_t pinConnectJ, uint8_t pinConnectK) {
  this->frogNumber = frogNumber;

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

void Frog::eventReceived(uint16_t index) {
  /**
   * Handle the test cycle start and stop events.
   */
  if (index == testStartEventIndex) {
    Serial.printf("\nFrog %d starting the testing cycle.", frogNumber);
  }
  if (index == testStopEventIndex) {
    Serial.printf("\nFrog %d stopping the testing cycle.", frogNumber);
  }

  // Compare receivedEventIndex with each of the four events for this frog.
  if (index == this->eventIndexConnectJ) {
    myMutex.setPinActive(this->pinConnectJ);
    this->currentState = State::CONNECTED_J;
  } else if (index == this->eventIndexConnectK) {
    myMutex.setPinActive(this->pinConnectK);
    this->currentState = State::CONNECTED_K;
  } else if (index == this->eventIndexDisconnectJ) {
    myMutex.setAllPinsInActive();
    this->currentState = State::DISCONNECTED;
  } else if (index == this->eventIndexDisconnectK) {
    myMutex.setAllPinsInActive();
    this->currentState = State::DISCONNECTED;
  }
}

bool Frog::eventIndexMatches(uint16_t index) {
  // Check for one of the testing event indexes.
  if ((index == this->testStartEventIndex) ||
      (index == this->testStopEventIndex)) return true;

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

void Frog::print() {
  Serial.printf("\neventIndexConnectJ=%#02X, eventIndexDisconnectJ=%#02X, eventIndexConnectK=%#02X, eventIndexDisconnectK=%#02X,", eventIndexConnectJ, eventIndexDisconnectJ, eventIndexConnectK, eventIndexDisconnectK);
}
