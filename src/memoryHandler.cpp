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
