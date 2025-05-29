#ifndef FROG_H
#define FROG_H

#include <Arduino.h>
#include "hw_mutex.h"
#include <vector>

/**
 * frog.h
 * 
 * The main code will support two frogs. Each frog will be a frog object.
 * 
 * Each frog has 3 events;-
 * - connect the frog to J
 * - connect the frog to K
 * - disconnect the frog from both J and K.
 */

 class FROG {
  public:
    /**
     * Setting the high or low active state
     * must be performed before the call to setPins() as that
     * call will set all pins to the inactive state.
     */
    void setPinsActiveHigh() {myMutex.setOutputsActiveHigh();}
    void setpinsActiveLow() {myMutex.setOutputsActiveLow();}

    void setPins(uint8_t pinConnectJ, uint8_t pinConnectK);
    void setEvents(uint16_t eventIndexConnectJ, uint16_t eventIndexConnectK, uint16_t eventIndexDisconnect);

    void process() {myMutex.process();}

    /**
     * Compares receivedEventIndex with those stored for this frog
     * and executes appropriately if any found.
     */
    void eventReceived(uint16_t receivedEventIndex);

  private:
    uint8_t pinConnectJ; // Setting this pin active will connect the frog to the J wire.
    uint8_t pinConnectK; // Setting this pin active will connect the frog to the K wire.
    uint16_t eventIndexConnectJ; // Receiving this event will set pinConnectJ active.
    uint16_t eventIndexConnectK; // Receiving this event will set pinConnectK active.
    uint16_t eventIndexDisconnect; // Receiving this event will set pinConnectJ and pinConnectK inactive.

    // A software object to ensure that the frog can only be connected to one of J and K at a time.
    HW_MUTEX myMutex;
 };
 
#endif
