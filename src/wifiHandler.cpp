#include "wifiHandler.h"

// TODO: Objectify
const char* ssid     = "UTTetudiants";
const char* password = "";

void initWifi(){
  DEBUG_PRINT("Connecting to ");
  DEBUG_PRINTLN(ssid);
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  DEBUG_PRINT("");
  DEBUG_PRINTLN("WiFi connected.");
  DEBUG_PRINTLN("IP address: ");
  DEBUG_PRINTLN(WiFi.localIP());
}
