#ifndef AIRQHANDLER_H
#define AIRQHANDLER_H

#include <stdint.h>
#include <Adafruit_BME680.h>
#include "global.h"

class GZ_AirSensor{
  public:
    // Constructeur
    GZ_AirSensor();

    // Accesseurs
    float    getTemperature();
    uint32_t getPressure();
    float    getHumidity();
    uint32_t getGas_resistance();

    // Methodes internes
    void init();
    void printReadings();
    String getInfos();

  private:
    Adafruit_BME680 _airSensor;
};

#endif
