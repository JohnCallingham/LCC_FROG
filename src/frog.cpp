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

void Frog::loop() {
  myMutex.loop();

  if (testing) {
    testLoop();
  }
}

void Frog::eventReceived(uint16_t index) {
  /**
   * Handle the test cycle start and stop events.
   */
  if (index == testStartEventIndex) {
    Serial.printf("\nFrog %d starting the testing cycle.", frogNumber);

    // Disconnect this frog from both J and K wires so that testing starts from a known condition.
    myMutex.setAllPinsInActive();

    // Set the first test.
    currentTest = CONNECT_J;

    // Set the timer so that testing starts immediately.
    testingTimer = millis();

    testing = true;
  }
  if (index == testStopEventIndex) {
    Serial.printf("\nFrog %d stopping the testing cycle.", frogNumber);

    // Disconnect this frog from both J and K wires in case testing has left the frog connected.
    myMutex.setAllPinsInActive();

    testing = false;
  }

  // Stop normal operation if testing.
  if (testing) return;

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

void Frog::testLoop() {

  if (millis() >= testingTimer) {
    // Time to move to the next part of the test cycle.
    switch (currentTest) {
      case CONNECT_J:
        // Start this cycle of the tests.
        // Connect this frog to the J wire for one second.
        myMutex.setPinActive(this->pinConnectJ);
        testingTimer = millis() + 1000;

        // Set the next test.
        currentTest = DISCONNECT_J;

        break;
      
      case DISCONNECT_J:
        // Start this cycle of the tests.
        // Disconnect this frog from the J wire for one second.
        myMutex.setAllPinsInActive();
        testingTimer = millis() + 1000;

        // Set the next test.
        currentTest = CONNECT_K;

        break;
      
      case CONNECT_K:
        // Start this cycle of the tests.
        // Connect this frog to the K wire for one second.
        myMutex.setPinActive(this->pinConnectK);
        testingTimer = millis() + 1000;

        // Set the next test.
        currentTest = DISCONNECT_K;

        break;
      
      case DISCONNECT_K:
        // Start this cycle of the tests.
        // Disconnect this frog from the K wire for one second.
        myMutex.setAllPinsInActive();
        testingTimer = millis() + 1000;

        // Set the next test.
        // Repeat from the start.
        currentTest = CONNECT_J;

        break;
      
      default:
        break;
    }
  }
}
