#ifndef WIFIHANDLER_H
#define WIFIHANDLER_H

#include <WiFi.h>
#include <WebServer.h>
#include "global.h"

#include "airQHandler.h"
#include "gpsHandler.h"

// Forward Declarations
class GZ_AirSensor;
class GZ_GPS;

// Declarations
class GZ_WebServer{
  public:
    // Constructeurs
    GZ_WebServer();
    GZ_WebServer(char* ssid, char* passwd, GZ_AirSensor* airSensor, GZ_GPS* gps);

    // Accesseurs
    int   getPort();
    void  setPort(int port);
    char* getSsid();
    void  setSsid(char* ssid);
    char* getPasswd();
    void  setPasswd(char* passwd);

    // Methodes internes
    void initWifi();
    void handleRoot();
    void handleJS();
    void handleJSON();
    void begin();
    void run();

  private:
    GZ_AirSensor* _airSensor;
    GZ_GPS*       _gps;

    char* _ssid   = "";
    char* _passwd = "";
    int   _port   = WEBSERVER_PORT;
    WebServer _server;
};


#endif
