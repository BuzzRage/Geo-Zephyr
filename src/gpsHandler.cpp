#include "gpsHandler.h"

#define GPSECHO 	false
#define GPSSerial Serial2

GZ_GPS::GZ_GPS(){
  _GPS  = new TinyGPSPlus();
  _timer = millis();
}

String GZ_GPS::getLatLng(){
  return String(_GPS->location.lat()/100)+";"+String(_GPS->location.lng()/100);
}

uint32_t GZ_GPS::getTimer(){
  return _timer;
}

void GZ_GPS::init(){
  GPSSerial.begin(GPS_BAUD);
}

void GZ_GPS::test(){
  while(GPSSerial.available() > 0)
    if(_GPS->encode(GPSSerial.read())){
      displayInfo();
    }

  if (millis() > 5000 && _GPS->charsProcessed() < 10){
    DEBUG_PRINTLN(F("No GPS detected: check wiring."));
    while(true);
  }
}

void GZ_GPS::displayInfo(){
  String dataString = "";
  dataString += (_GPS->date.isValid() ? String(_GPS->date.day())+"/"+String(_GPS->date.month())+"/"+String(_GPS->date.year()) + ",": "null,");

  if(_GPS->time.isValid()){
    int heure = _GPS->time.hour()+2;
    if (heure < 10)                 dataString += "0";
    dataString += String(heure) + ":";
    if (_GPS->time.minute() < 10)     dataString += "0";
    dataString += String(_GPS->time.minute()) + ":";
    if (_GPS->time.second() < 10)     dataString += "0";
    dataString += String(_GPS->time.second());
  }
  else{
    dataString += "null";
  }

  String lat = String(_GPS->location.lat(),6);
  String lng = String(_GPS->location.lng(),6);
  dataString += (_GPS->location.isValid() ? ","+lat+","+lng : ",null");
  Serial.println("data: " + dataString);
}
