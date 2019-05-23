#include "gpsHandler.h"

#define GPSECHO 	false
#define GPSSerial Serial2

GZ_GPS::GZ_GPS(){
  _GPS  = new Adafruit_GPS(&GPSSerial);
  _timer = millis();
}

String GZ_GPS::getLatLng(){
  return String(_GPS->latitude/100)+";"+String(_GPS->longitude/100);
}

uint32_t GZ_GPS::getTimer(){
  return _timer;
}

void GZ_GPS::init(){
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.println("Adafruit GPS library basic test!");

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  _GPS->begin(9600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  _GPS->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  _GPS->sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  _GPS->sendCommand(PGCMD_ANTENNA);
  delay(1000);

  // Ask for firmware version
  GPSSerial.println(PMTK_Q_RELEASE);
}

void GZ_GPS::test(){
  // read data from the GPS in the 'main loop'
   char c = _GPS->read();
   // if you want to debug, this is a good time to do it!
   if (GPSECHO)
     if (c) Serial.print(c);
   // if a sentence is received, we can check the checksum, parse it...
   if (_GPS->newNMEAreceived()) {
     // a tricky thing here is if we print the NMEA sentence, or data
     // we end up not listening and catching other sentences!
     // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
     Serial.println(_GPS->lastNMEA()); // this also sets the newNMEAreceived() flag to false
     if (!_GPS->parse(_GPS->lastNMEA())) // this also sets the newNMEAreceived() flag to false
       return; // we can fail to parse a sentence in which case we should just wait for another
   }
   // if millis() or timer wraps around, we'll just reset it
   if (_timer > millis()) _timer = millis();

   // approximately every 2 seconds or so, print out the current stats
   if (millis() - _timer > 2000) {
     _timer = millis(); // reset the timer
     Serial.print("\nTime: ");
     Serial.print(_GPS->hour+2, DEC); Serial.print(':');
     Serial.print(_GPS->minute, DEC); Serial.print(':');
     Serial.println(_GPS->seconds, DEC); //Serial.print('.');
     //Serial.println(GPS.milliseconds);
     Serial.print("Date: ");
     Serial.print(_GPS->day, DEC); Serial.print('/');
     Serial.print(_GPS->month, DEC); Serial.print("/20");
     Serial.println(_GPS->year, DEC);
     Serial.print("Fix: "); Serial.print((int)_GPS->fix);
     Serial.print(" quality: "); Serial.println((int)_GPS->fixquality);
     if(_GPS->fix){
       Serial.print("Location: ");
       // GPS.latitude format: ddmm.mmmm
       // GPS.longitude format: dddmm.mmm
       float latitude = _GPS->latitude/100.0;
       float longitude = _GPS->longitude/100.0;

       Serial.print(latitude, 6); Serial.print(_GPS->lat);
       Serial.print(", ");
       Serial.print(longitude, 6); Serial.println(_GPS->lon);
       Serial.print("Speed (knots): "); Serial.println(_GPS->speed);
       Serial.print("Angle: "); Serial.println(_GPS->angle);
       Serial.print("Altitude: "); Serial.println(_GPS->altitude);
       Serial.print("Satellites: "); Serial.println((int)_GPS->satellites);
     }
 }
}
