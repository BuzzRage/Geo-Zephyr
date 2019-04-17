#include <Arduino.h>
#include <Adafruit_BME680.h>
#include <Adafruit_GPS.h>

#include "global.h"
#ifdef WIFI_ENABLED
  #include "wifiHandler.h"
#endif
//#include "airQHandler.h" // Not ready yet. Objectification needed

// Il est possible de définir les pin que l'on veut pour le UART via:
//HardwareSerial GPS_Serial(2);
//#define GPSSerial GPS_Serial

// what's the name of the hardware serial port? Maybe Serial2 for RX 16, TX 17
#define GPSSerial Serial2
// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);
// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

uint32_t timer = millis();


Adafruit_BME680 bme;
#ifdef WIFI_ENABLED
  WiFiServer server(WEBSERVER_PORT);
#endif
String header = "";

void initBme();
void testBME();
void initGPS();
void testGPS();

void setup(){
  Serial.begin(SERIAL_SPEED);

  #ifdef WIFI_ENABLED
    initWifi();
    server.begin();
  #endif
  initBme();
  initGPS();
}

void loop() {
  //delay(2000);
  //testBME();
  testGPS();

  #ifdef WIFI_ENABLED
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

void initGPS(){
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.println("Adafruit GPS library basic test!");

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);

  // Ask for firmware version
  GPSSerial.println(PMTK_Q_RELEASE);
}

void testGPS(){
  // read data from the GPS in the 'main loop'
   char c = GPS.read();
   // if you want to debug, this is a good time to do it!
   if (GPSECHO)
     if (c) Serial.print(c);
   // if a sentence is received, we can check the checksum, parse it...
   if (GPS.newNMEAreceived()) {
     // a tricky thing here is if we print the NMEA sentence, or data
     // we end up not listening and catching other sentences!
     // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
     Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
     if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
       return; // we can fail to parse a sentence in which case we should just wait for another
   }
   // if millis() or timer wraps around, we'll just reset it
   if (timer > millis()) timer = millis();

   // approximately every 2 seconds or so, print out the current stats
   if (millis() - timer > 2000) {
     timer = millis(); // reset the timer
     Serial.print("\nTime: ");
     Serial.print(GPS.hour, DEC); Serial.print(':');
     Serial.print(GPS.minute, DEC); Serial.print(':');
     Serial.print(GPS.seconds, DEC); Serial.print('.');
     Serial.println(GPS.milliseconds);
     Serial.print("Date: ");
     Serial.print(GPS.day, DEC); Serial.print('/');
     Serial.print(GPS.month, DEC); Serial.print("/20");
     Serial.println(GPS.year, DEC);
     Serial.print("Fix: "); Serial.print((int)GPS.fix);
     Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
     if (GPS.fix) {
       Serial.print("Location: ");
       Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
       Serial.print(", ");
       Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
       Serial.print("Speed (knots): "); Serial.println(GPS.speed);
       Serial.print("Angle: "); Serial.println(GPS.angle);
       Serial.print("Altitude: "); Serial.println(GPS.altitude);
       Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
     }
 }
}
