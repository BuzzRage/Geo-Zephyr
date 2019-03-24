#include <Arduino.h>
#include "global.h"

void setup() {
  Serial.begin(SERIAL_SPEED);
  Serial.println("Test reussi !");
}

void loop() {
  Serial.println("Test reussi !");
  delay(2000);
}
