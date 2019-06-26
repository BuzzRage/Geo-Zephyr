#ifndef AIRQHANDLER_H
#define AIRQHANDLER_H

#include <stdint.h>
#include <Adafruit_BME680.h>
#include "global.h"

#define HUM_REF   40.0

class GZ_AirSensor{
  public:
    // Constructeur
    GZ_AirSensor();

    // Accesseurs
    float    getTemperature();
    float    getPressure();
    float    getHumidity();
    float    getGas_resistance();
    float    getAirQScore();
    String   getIAQ(float score);

    // Methodes internes
    void init();
    int performReading();
    void printReadings();
    String getInfos();

  private:
    Adafruit_BME680 _airSensor;
    float airQ_score;
    float hum_score, gas_score;
    float gas_reference;
};

#endif
