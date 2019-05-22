#ifndef WIFIHANDLER_H
#define WIFIHANDLER_H

#include <WiFi.h>
#include "global.h"

class WebServer{
  public:
    // Constructeurs
    WebServer();
    WebServer(char* ssid, char* passwd);

    // Accesseurs
    int getPort();
    void setPort(int port);
    char* getSsid();
    void setSsid(char* ssid);
    char* getPasswd();
    void setPasswd(char* passwd);

    // Methodes internes
    void initWifi();
    void begin();
    void run(String str);

  private:
    char* _ssid   = "";
    char* _passwd = "";
    int         _port   = WEBSERVER_PORT;
    WiFiServer  _server;
};


#endif
