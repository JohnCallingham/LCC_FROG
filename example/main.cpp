#include <Arduino.h>
#include "frog.h"

#define NUM_FROG 2
#define NUM_EVENT NUM_FROG * 3 // Each frog has connectJ, connectK and disconnect events.

// Create two Frog objects and set their pins.
Frog frog0(11, 12);
Frog frog1(21, 22);

// Declare an array of pointers to the Frog objects.
Frog *frog[NUM_FROG];

...

/**
 * userState() is called when JMRI queries the state of an event index.
 */
enum evStates { VALID=4, INVALID=5, UNKNOWN=7 };
uint8_t userState(uint16_t index) {
  Serial.printf("\n%d In userState() for event index = %d", millis(), index);

  // Determine which Frog object has this event index.
  for (uint8_t i=0; i<NUM_FROG; i++) {
    Serial.printf("\nChecking for matching Frog %d", i);
    if (frog[i]->eventIndexMatchesThisFrog(index)) {
      // This FROG object has this event index.
      Serial.printf("\nChecking for matching event");
      if (frog[i]->eventIndexMatchesCurrentState(index)) {
        return VALID;
      } else {
        return INVALID;
      }
    }
  }

  return UNKNOWN; // In case index is not recognised.
}

// ===== Process Consumer-eventIDs =====
void pceCallback(uint16_t index) {
  // Invoked when an event is consumed; drive pins as needed
  // from index of all events.

  // Determine which frog this event index is for.
  for (uint8_t i=0; i<NUM_FROG; i++) {
    if (frog[i]->eventIndexMatchesThisFrog(index)) {
      // This Frog object has this event index.

      // Set the frog's pins as required.
      frog[i]->eventReceived(index);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(5000);
  Serial.printf("\n ESP32-2TOTI_Wifi");

  // Configure the built in blue LED and turn it off.
  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_BLUE, HIGH);

  NodeID nodeid(NODE_ADDRESS);       // this node's nodeid
  Olcb_init(nodeid, RESET_TO_FACTORY_DEFAULTS);

  // Store the pointers to the frog objects in the frog array.
  frog[0] = &frog0;
  frog[1] = &frog1;

  // Initialise all Frog objects.
  for (uint8_t i=0; i<NUM_FROG; i++) {
    frog[i]->setDelaymS(5000); // The time that both J and K are disconnected when changing from one to the other.
    frog[i]->setEvents((i*3)+0, (i*3)+1, (i*3)+2); // Assumes that the first frog event index is 0.
    frog[i]->print();
  }

  dP("\n initialization finished");

  dP("\n NUM_EVENT="); dP(NUM_EVENT);
}

void loop() {
...
  // Process any frog timeouts.
  for (uint8_t i=0; i<NUM_FROG; i++) {
    frog[i]->process();
  }
...
}
