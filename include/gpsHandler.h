#ifndef GPSHANDLER_H
#define GPSHANDLER_H

#include <Adafruit_GPS.h>
#include <Arduino.h>
#include "global.h"

class GZ_GPS{
  public:
    // Constructeurs
    GZ_GPS();

    // Accesseurs
    String   getLatLng();
    uint32_t getTimer();

    // Methodes internes
    void init();
    void test();

  private:
    Adafruit_GPS* _GPS;
    uint32_t      _timer;
};

#endif
