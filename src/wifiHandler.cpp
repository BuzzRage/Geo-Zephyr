#include "wifiHandler.h"

GZ_WebServer::GZ_WebServer(){

}

GZ_WebServer::GZ_WebServer(char* ssid, char* passwd){
  _ssid   = ssid;
  _passwd = passwd;
}

void GZ_WebServer::initWifi(){
  DEBUG_PRINT("Connecting to ");
  DEBUG_PRINTLN(_ssid);
  WiFi.begin(_ssid,_passwd);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  DEBUG_PRINT("");
  DEBUG_PRINTLN("WiFi connected.");
  DEBUG_PRINTLN("IP address: ");
  DEBUG_PRINTLN(WiFi.localIP());
}

void GZ_WebServer::begin(){
  _server.begin(_port);
}

void GZ_WebServer::run(String str){
  WiFiClient client = _server.available();
  String header = "";
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

            client.println(str);

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

int GZ_WebServer::getPort(){
  return this->_port;
}

void GZ_WebServer::setPort(int port){
  this->_port = port;
}

char* GZ_WebServer::getSsid(){
  return this->_ssid;
}

void GZ_WebServer::setSsid(char* ssid){
  this->_ssid = ssid;
}

char* GZ_WebServer::getPasswd(){
  return this->_passwd;
}

void GZ_WebServer::setPasswd(char* passwd){
  this->_passwd = passwd;
}
