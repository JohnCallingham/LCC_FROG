#ifndef FROG_H
#define FROG_H

#include <Arduino.h>

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
    void setPins(uint8_t pinConnectJ, uint8_t pinConnectK);
    void setEvents(uint16_t eventIndexConnectJ, uint16_t eventIndexConnectK, uint16_t eventIndexDisconnect);




  private:
    uint8_t pinConnectJ; // Setting this pin active will connect the frog to the J wire.
    uint8_t pinConnectK; // Setting this pin active will connect the frog to the K wire.
    uint16_t eventIndexConnectJ; // Receiving this event will connect the frog to the J wire.
    uint16_t eventIndexConnectK; // Receiving this event will connect the frog to the K wire.
    uint16_t eventIndexDisconnect; // Receiving this event will disconnect the frog from both J and K.


 };
 
#endif
