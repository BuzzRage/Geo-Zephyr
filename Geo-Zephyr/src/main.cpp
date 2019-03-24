#include <Arduino.h>
#include <WiFi.h>
#include "global.h"

// Enter your credentials here
const char* ssid     = "";
const char* password = "";

WiFiServer server(WEBSERVER_PORT);
String header = "";

void setup() {
  Serial.begin(SERIAL_SPEED);
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
  server.begin();
}

void loop() {
  delay(2000);

  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    DEBUG_PRINTLN("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        DEBUG_WRITE(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

                        // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            //client.println("<head></head>");

            // Web Page Heading
            client.println("<body><h1>GeoZéphyr</h1>");
            client.println("<p>Bienvenu sur cette page !</p>");
            client.println("</body></html>");
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    DEBUG_PRINTLN("Client disconnected.");
    DEBUG_PRINTLN("");
  }
}
