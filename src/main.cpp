#include <Arduino.h>
#include <Adafruit_BME680.h>

#include "global.h"
#include "wifiHandler.h"
//#include "airQHandler.h" // Not ready yet. Objectification needed

Adafruit_BME680 bme;
WiFiServer server(WEBSERVER_PORT);
String header = "";

void initBme();
void testBME();

void setup(){
  Serial.begin(SERIAL_SPEED);

  initWifi();
  server.begin();
  initBme();
}

void loop() {
  delay(2000);
  testBME();
  WiFiClient client = server.available();

  if(client){
    DEBUG_PRINTLN("New Client.");
    String currentLine = "";                // make a String to hold incoming data from the client
    while(client.connected()){
      if(client.available()){
        char c = client.read();
        DEBUG_WRITE(c);
        header += c;
        if(c == '\n'){                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            client.println("<!DOCTYPE html><html>");
            //client.println("<head></head>");

            // Web Page Heading
            client.println("<body><h1>GeoZéphyr</h1>");
            client.println("<p>Bienvenu sur cette page !</p>");
            client.println("<p>Il fait "+String(bme.temperature)+"°C</p>");
            client.println("<p>La pression atmosphérique est de "+String(bme.pressure / 100.0)+"hPa ");
            client.println("avec un taux de "+String(bme.humidity)+"%</p>");
            client.println("<p>Gaz = "+String(bme.gas_resistance/1000)+"kOhms</p>");
            client.println("</body></html>");
            // The HTTP response ends with another blank line
            client.println();
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    header = "";
    client.stop();
    DEBUG_PRINTLN("Client disconnected.");
    DEBUG_PRINTLN("");
  }
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
 delay(2000);
}
