#ifndef FROG_H
#define FROG_H

/**
 * frog.h
 * 
 * The Frog class represents one frog. It provides the following functionlity;-
 * 1. Initialise the object without sending any events.
 * 2. CANNOT DO THIS AS CONSUMED EVENTS CANNOT BE SENT!!! When the hub is connected it sends an event to indicate its current state which can be one of;-
 *  a. frog connected to the J wire.
 *  b. frog connected to the K wire.
 *  c. frog disconnected from both the J and K wires.
 * 3. When an event is consumed it provides the appropriate action. The events and associated actions are;-
 *  a. eventIndexConnectJ - causes the frog to be connected to the J wire.
 *  b. eventIndexConnectK - causes the frog to be connected to the K wire.
 *  c. eventIndexDisconnect - causes the frog to disconnect from both the J and K wires.
 * 4. Responds to a query from JMRI for current state based on event index. This uses function userState().
 */

 /**
  * TO DO: need to maintain the current state of the frog - J, K or disconnected.
  * This will be used to respond to a JMRI request for state.
  * What to return if in the process of waiting for the timeout to expire? Disconnected need to ensure that this state is kept up to date 

  */

#include <Arduino.h>
#include "hw_mutex.h"
#include <vector>

 class Frog {
  public:
    void initialise() { this->currentState = State::DISCONNECTED; }

    /**
     * Setting the high or low active state
     * must be performed before the call to .setPins() as that
     * call will set all pins to the inactive state.
     */
    void setPinsActiveHigh() {myMutex.setOutputsActiveHigh();}
    void setPinsActiveLow() {myMutex.setOutputsActiveLow();}

    void setDelaymS(unsigned long delaymS) {myMutex.setDelaymS(delaymS);}

    void setPins(uint8_t pinConnectJ, uint8_t pinConnectK);
    void setEvents(uint16_t eventIndexConnectJ, uint16_t eventIndexConnectK, uint16_t eventIndexDisconnect);

    int getEventForCurrentState(); // Used when the hub is (re)connected.
    // BUT: don't think that we can send an event which is a consumer event!!!

    /**
     * To be called from loop() to enable a delay in switching from one output to another.
     */
    void process() {myMutex.process();}

    /**
     * Compares receivedEventIndex with those stored for this frog
     * and executes appropriately if any found.
     */
    void eventReceived(uint16_t receivedEventIndex);

  private:
    enum State { CONNECTED_J, CONNECTED_K, DISCONNECTED };
    uint8_t pinConnectJ; // Setting this pin active will connect the frog to the J wire.
    uint8_t pinConnectK; // Setting this pin active will connect the frog to the K wire.
    uint16_t eventIndexConnectJ; // Receiving this event will set pinConnectJ active.
    uint16_t eventIndexConnectK; // Receiving this event will set pinConnectK active.
    uint16_t eventIndexDisconnect; // Receiving this event will set pinConnectJ and pinConnectK inactive.
    State currentState;

    // A software object to ensure that the frog can only be connected to one of J and K at a time.
    HwMutex myMutex;
 };
 
#endif
