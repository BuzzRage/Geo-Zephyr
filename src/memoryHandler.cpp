#include "memoryHandler.h"

GZ_SDCard::GZ_SDCard() {
}

String GZ_SDCard::readFile(String path) {
  String data;

  File file = SD.open(path);
  if (!file) {
    return data;
  }

  data = "";
  while (file.available()) {
   data += String((char)file.read());
 }
 file.close();
 return data;
}

byte GZ_SDCard::writeFile(String path, String message) {
  File file = SD.open(path, FILE_WRITE);
  if (!file) {
    return false;
  }
  if (file.print(message)) {
    file.close();
    return true;
  } else {
    file.close();
    return false;
  }
}

byte GZ_SDCard::appendFile(String path, String message) {
  File file = SD.open(path, FILE_APPEND);
  if(!file){
    return false;
  }

  if(file.print(message)){
    file.close();
    return true;
  }else{
    file.close();
    return false;
  }
}

byte GZ_SDCard::createDir(String path) {
  if (SD.mkdir(path)) {
    return true;
  } else {
    return false;
  }
}

byte GZ_SDCard::removeDir(String path) {
  if (SD.rmdir(path)) {
    return true;
  } else {
    return false;
  }
}

bool GZ_SDCard::fileExists(String path) {
  File file = SD.open(path);
  if (!file) {
    return false;
  } else {
    file.close();
    return true;
  }
}

// void write_CSV_entry(){
//
//   if(!SD.exists(FILENAME)){
//     dataFile = SD.open(FILENAME, FILE_WRITE);
//     for (byte k = 0; k < strlen_P(csv_header); k++) {
//       dataFile.write(pgm_read_byte_near(csv_header + k));
//     }
//     dataFile.println();
//     DISPLAY_PRINTLN(F(String(FILENAME) + " created"));
//     nbpts = 0;
//   }
//   else
//     dataFile = SD.open(FILENAME, FILE_WRITE);
//
//   if(dataFile){
//     String dataString = "";
//     dataString += (gps.date.isValid() ? String(gps.date.day())+"/"+String(gps.date.month())+"/"+String(gps.date.year()) + ",": "null,");
//
//     if(gps.time.isValid()){
//       int heure = (gps.time.hour()+2)%24;
//       if (heure < 10)                 dataString += '0';
//       dataString += String(heure) + ':';
//       if (gps.time.minute() < 10)     dataString += '0';
//       dataString += String(gps.time.minute()) + ':';
//       if (gps.time.second() < 10)     dataString += '0';
//       dataString += String(gps.time.second());
//     }
//     else{
//       dataString += "nAn";
//     }
//
//     float lat = gps.location.lat();
//     float lng = gps.location.lng();
//
//     if(gps.location.isValid())
//       dataString += ","+String(lat,6)+","+String(lng,6);
//     else
//       dataString += ",nAn";
//     dataString += ","+String(gps.altitude.meters());
//     dataString += ","+String(gps.satellites.value());
//     dataString += ","+String(gps.hdop.value());
//
//     DISPLAY_PRINT(F(String(FILENAME) + " will be written with:  "));
//     DISPLAY_PRINTLN(F(dataString));
//
//     dataFile.println(dataString);
//     dataFile.close();
//
//     nbpts++;
//     DISPLAY_PRINTLN(F(String(FILENAME) + " wrotten"));
//   }
//   else
//     DISPLAY_PRINTLN(F("Error opening "+String(FILENAME)));
// }
//
// void upload_CSV_file(){
//   dataFile = SD.open(FILENAME, FILE_READ);
//   if(dataFile){
//     DISPLAY_PRINTLN(F(String(FILENAME)+" will be read"));
//
//     while(dataFile.available())
//       Serial.write(dataFile.read());
//     dataFile.close();
//
//     DISPLAY_PRINTLN(F(String(FILENAME)+" read"));
//   }
//   else
//     DISPLAY_PRINTLN(F("Can't open "+String(FILENAME)));
// }
