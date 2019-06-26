#include <Arduino.h>
#include "global.h"

#ifdef GPS_ENABLED
  #include "gpsHandler.h"
  GZ_GPS GPS;
#endif

#ifdef AIRQ_ENABLED
  #include "airQHandler.h"
  GZ_AirSensor airSensor;
#endif

#if defined WIFI_ENABLED && defined GPS_ENABLED && defined AIRQ_ENABLED
  #include "wifiHandler.h"
  GZ_WebServer webServer("Musicmen","Kafe!nay", &airSensor, &GPS);
#endif

uint32_t timestamp = 0;

void setup(){
  Serial.begin(SERIAL_SPEED);

  #ifdef WIFI_ENABLED
    webServer.initWifi();
    webServer.begin();
  #endif

  #ifdef AIRQ_ENABLED
    airSensor.init();
  #endif

  #ifdef GPS_ENABLED
    GPS.init();
  #endif

}

void loop() {

  #ifdef AIRQ_ENABLED
    airSensor.performReading();
  #endif

  #ifdef GPS_ENABLED
    GPS.run();
  #endif

  #ifdef WIFI_ENABLED
    webServer.run();
  #endif

  if(millis() - timestamp > 1000){
    webServer.write_CSV_entry();
    timestamp = millis();
  }
}
