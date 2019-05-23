#include <Arduino.h>
#include <Adafruit_BME680.h>

#include "global.h"

#ifdef WIFI_ENABLED
  #include "wifiHandler.h"
  GZ_WebServer webServer("ssid","password");
#endif


#ifdef GPS_ENABLED
  #include "gpsHandler.h"
  GZ_GPS GPS;
#endif

Adafruit_BME680 bme;

void initBme();
void testBME();

void setup(){
  Serial.begin(SERIAL_SPEED);

  #ifdef WIFI_ENABLED
    webServer.initWifi();
    webServer.begin();
  #endif

  initBme();

  #ifdef GPS_ENABLED
    GPS.init();
  #endif
}

void loop() {
  //delay(2000);
  //testBME();

  #ifdef GPS_ENABLED
    GPS.test();
  #endif

  #ifdef WIFI_ENABLED
    String str = "<!DOCTYPE html><html> \
    <head></head> \
    <body> \
      <h1>GeoZéphyr</h1> \
      <p>Bienvenu sur cette page !</p> \
      <p>Il fait "+String(bme.temperature)+"°C</p> \
      <p>La pression atmosphérique est de "+String(bme.pressure / 100.0)+"hPa \
         avec un taux de "+String(bme.humidity)+"%</p> \
      <p>Gaz = "+String(bme.gas_resistance/1000)+"kOhms</p> \
    </body> \
    </html>";
    webServer.run(str);
  #endif
}

void initBme(){
  Serial.println("Air quality sensor initialization.");

  if (!bme.begin(I2C_ADDR)) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }
  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}

void testBME(){
  if (! bme.performReading()) {
   Serial.println("Failed to perform reading :(");
   return;
 }
 Serial.print("Temperature = ");
 Serial.print(bme.temperature);
 Serial.println(" *C");

 Serial.print("Pressure = ");
 Serial.print(bme.pressure / 100.0);
 Serial.println(" hPa");

 Serial.print("Humidity = ");
 Serial.print(bme.humidity);
 Serial.println(" %");

 Serial.print("Gas = ");
 Serial.print(bme.gas_resistance / 1000.0);
 Serial.println(" KOhms");

 Serial.print("Approx. Altitude = ");
 Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
 Serial.println(" m");

 Serial.println();
}
