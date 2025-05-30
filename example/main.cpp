#include <Arduino.h>
#include "frog.h"

// Create a Frog object.
Frog myFrog;

void setup() {
  Serial.begin(115200);
  delay(5000);

  Serial.printf("\nstarted");

  /**
   * 
   * Set up the pins to be active high or active low.
   * This must be done before calling .setPins() as that call will
   * set all pins to the current inactive state.
   */
  myFrog.setPinsActiveHigh();

  // Set up the pins which connect the J and K wires to the frog.
  uint8_t pinConnectJ = 11;
  uint8_t pinConnectK = 12;
  myFrog.setPins(pinConnectJ, pinConnectK);

  // Set up the events which control the J and K wires.
  uint16_t eventIndexConnectJ = 21;
  uint16_t eventIndexConnectK = 22;
  uint16_t eventIndexDisconnect = 23;  
  myFrog.setEvents(eventIndexConnectJ, eventIndexConnectK, eventIndexDisconnect);

  myFrog.setDelaymS(50); // The default is 100 mS.

  // Simulate an event being received.
  myFrog.eventReceived(eventIndexConnectK);
}

void loop() {
  myFrog.process();
}
