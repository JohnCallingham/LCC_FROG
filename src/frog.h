#ifndef FROG_H
#define FROG_H

/**
 * frog.h
 * 
 * The Frog class represents one frog. It provides the following functionlity;-
 * 1. The outputs can be configured to be active high or active low.
 * 2. When the frog is switched from J to K there is a configurable delay
 *     when both outputs are inactive.
 * 3. When an event is consumed it provides the appropriate action.
 *  The events and associated actions are;-
 *  a. eventIndexConnectJ - causes the frog to be connected to the J wire.
 *  b. eventIndexConnectK - causes the frog to be connected to the K wire.
 *  c. eventIndexDisconnectJ - causes the frog to disconnect from both the J and K wires.
 *  c. eventIndexDisconnectK - causes the frog to disconnect from both the J and K wires.
 * 4. Responds to a query from JMRI for current state based on event index.
 */

#include <Arduino.h>
#include "hw_mutex.h"
#include "LCC_Node_Component_Base.h"
#include <vector>

class Frog : public LCC_Node_Component_Base  {
  public:
    /**
     * The constructor sets the active state to active low and then
     * sets all pins inactive.
     */
    Frog(uint8_t frogNumber, uint8_t pinConnectJ, uint8_t pinConnectK);

    /**
     * Allows the active state to be changed after the frog object is created.
     */
    void setPinsActiveHigh() {myMutex.setOutputsActiveHigh();}
    void setPinsActiveLow() {myMutex.setOutputsActiveLow();}

    void setDelaymS(unsigned long delaymS) {myMutex.setDelaymS(delaymS);}

    void setEvents(uint16_t eventIndexConnectJ, uint16_t eventIndexDisconnectJ, uint16_t eventIndexConnectK, uint16_t eventIndexDisconnectK);

    /**
     * Returns true if index matches one of this object's events, else false.
     */
    bool eventIndexMatches(uint16_t index) override;

    /**
     * Returns true if index matches the current state, else false.
     * e.g. if index == eventIndexOccupied and the current state is occupied, then return true.
     */
    bool eventIndexMatchesCurrentState(uint16_t index) override;

    /**
     * This function is not used in the Frog class,
     *  but is required to override the base class function.
     */
    void sendEventsForCurrentState() override {};

    /**
     * To be called from loop() to enable a delay in switching from one output to another.
     * Also allows a testing cycle to be run when required.
     */
    // void loop() { myMutex.loop(); }
    void loop();
 
    /**
     * Compares index with those stored for this frog
     * and executes appropriately if any found.
     */
    void eventReceived(uint16_t index);

    bool isConnectedJ() { return (currentState == State::CONNECTED_J) ? true : false; }

    bool isConnectedK() { return (currentState == State::CONNECTED_K) ? true : false; }

    void print();

  private:
    uint8_t frogNumber;

    enum State { CONNECTED_J, CONNECTED_K, DISCONNECTED };
    State currentState;

    uint8_t pinConnectJ; // Setting this pin active will connect the frog to the J wire.
    uint8_t pinConnectK; // Setting this pin active will connect the frog to the K wire.

    uint16_t eventIndexConnectJ; // Receiving this event will set pinConnectJ active.
    uint16_t eventIndexDisconnectJ; // Receiving this event will set pinConnectJ inactive.
    uint16_t eventIndexConnectK; // Receiving this event will set pinConnectK active.
    uint16_t eventIndexDisconnectK; // Receiving this event will set pinConnectK inactive.

    // A software object to ensure that the frog can only be connected to one of J and K at a time.
    HwMutex myMutex;

    bool testing = false;
    unsigned long testingTimer; // Contains the value of millis() for the next change in the testing cycle.

    enum Test { CONNECT_J, DISCONNECT_J, CONNECT_K, DISCONNECT_K };
    Test currentTest;

    void testLoop();
};
 
#endif
