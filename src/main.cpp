#include <Arduino.h>
#include "global.h"

#ifdef WIFI_ENABLED
  #include "wifiHandler.h"
  GZ_WebServer webServer("ssid","password");
#endif

#ifdef GPS_ENABLED
  #include "gpsHandler.h"
  GZ_GPS GPS;
#endif

#ifdef AIRQ_ENABLED
  #include "airQHandler.h"
  GZ_AirSensor airSensor;
#endif


String get_CSV_data(){
  String gpsInfos = GPS.getInfos();
  String bmeInfos = airSensor.getInfos();
  if(bmeInfos != "0" and gpsInfos != "0")
    return gpsInfos + "," + bmeInfos;
  else
    return "0";
}


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
    airSensor.printReadings();
  #endif

  #ifdef GPS_ENABLED
    GPS.test();
  #endif

  #ifdef WIFI_ENABLED
    String str = "<!DOCTYPE html><html> \
    <head><meta http-equiv='Content-Type' content='text/html; charset=UTF-8' /></head> \
    <body> \
      <h1>GeoZéphyr</h1> \
      <p>Bienvenu sur cette page !</p> \
      <p>Il fait "+String(airSensor.getTemperature())+"°C</p> \
      <p>La pression atmosphérique est de "+String(airSensor.getPressure() / 100.0)+"hPa \
         avec un taux de "+String(airSensor.getHumidity())+"%</p> \
      <p>Gaz = "+String(airSensor.getGas_resistance()/1000)+"kOhms</p> \
      <p>CSV entry = "+get_CSV_data()+"\
    </body> \
    </html>";
    webServer.run(str);
  #endif
}
