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
 *  c. eventIndexDisconnectJ - causes the frog to disconnect from both the J and K wires.
 *  c. eventIndexDisconnectK - causes the frog to disconnect from both the J and K wires.
 * 4. Responds to a query from JMRI for current state based on event index. This uses function userState().
 * 
 * Electrical;-
 *
 * The ESP32 pin 2 (3V3 OUT) is 3.25/3.26 V.
 * The ESP32 pin 30 (Frog1 J) is 3.25 V when inactive and 0.00 V when active.
 * 
 * The LED in the TLP241 has a forward voltage drop of between 1.1 V and 1.4 V (typical is 1.27 V) at a forward current of 10 mA.
 * Using a value of 220R for the series resistor gives an LED current of between 9.77 mA and 8.41 mA.
 * The LED has a recommended operating current of 7.5 mA, with a maximum of 19.5 mA. Absolute maximum is 30 mA.
 * 
 * The GPIOs on the Nano ESP32 can handle source currents up to 40 mA, and sink currents up to 28 mA.
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
    Frog(uint8_t pinConnectJ, uint8_t pinConnectK);

    /**
     * Allows the active state to be changed after the frog object is created.
     */
    void setPinsActiveHigh() {myMutex.setOutputsActiveHigh();}
    void setPinsActiveLow() {myMutex.setOutputsActiveLow();}

    void setDelaymS(unsigned long delaymS) {myMutex.setDelaymS(delaymS);}

    void setEvents(uint16_t eventIndexConnectJ, uint16_t eventIndexDisconnectJ, uint16_t eventIndexConnectK, uint16_t eventIndexDisconnectK);

    int getEventForCurrentState(); // Used when the hub is (re)connected.
    // BUT: don't think that we can send an event which is a consumer event!!!

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
    void sendEventsForCurrentState() override;

    /**
     * To be called from loop() to enable a delay in switching from one output to another.
     */
    void process() { myMutex.process(); }

    /**
     * Compares receivedEventIndex with those stored for this frog
     * and executes appropriately if any found.
     */
    void eventReceived(uint16_t receivedEventIndex);

    bool isConnectedJ() { return (currentState == State::CONNECTED_J) ? true : false; }

    bool isConnectedK() { return (currentState == State::CONNECTED_K) ? true : false; }

    void print();

  private:
    enum State { CONNECTED_J, CONNECTED_K, DISCONNECTED };
    uint8_t pinConnectJ; // Setting this pin active will connect the frog to the J wire.
    uint8_t pinConnectK; // Setting this pin active will connect the frog to the K wire.
    uint16_t eventIndexConnectJ; // Receiving this event will set pinConnectJ active.
    uint16_t eventIndexDisconnectJ; // Receiving this event will set pinConnectJ inactive.
    uint16_t eventIndexConnectK; // Receiving this event will set pinConnectK active.
    uint16_t eventIndexDisconnectK; // Receiving this event will set pinConnectK inactive.
    State currentState;

    // A software object to ensure that the frog can only be connected to one of J and K at a time.
    HwMutex myMutex;
 };
 
#endif
