#ifndef MEMORYHANDLER_H
#define MEMORYHANDLER_H

#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include "global.h"

class GZ_SDCard {
  public:
      GZ_SDCard();
      String readFile(String path);
      byte writeFile(String path, String message);
      byte appendFile(String path, String message);
      byte createDir(String path);
      byte removeDir(String path);
      bool fileExists(String path);

      inline bool begin(int cs) {
          return SD.begin(cs);
      }
      inline bool begin() {
          return SD.begin();
      }
      inline void end() {
          SD.end();
      }
};

#endif
