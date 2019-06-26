#include "wifiHandler.h"

GZ_WebServer::GZ_WebServer(){

}

GZ_WebServer::GZ_WebServer(char* ssid, char* passwd, GZ_AirSensor* airSensor, GZ_GPS* gps) :
_airSensor(airSensor),
_gps(gps)
{
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
  _server.on("/sensors.json"        ,std::bind(&GZ_WebServer::handleJSON, this));

  _server.onNotFound(                std::bind(&GZ_WebServer::handleRoot, this));
  _server.begin(_port);
}

void GZ_WebServer::run(){
    _server.handleClient();
}

bool GZ_WebServer::loadFromMemoryCard(String path) {
  String mediaType = "text/plain";
  path = ROOT_PATH + path;
  Serial.println(path);
  if (path.endsWith("/")) path += "index.html";

  if (path.endsWith(".html"))       mediaType = "text/html";
  else if (path.endsWith(".css"))   mediaType = "text/css";
  else if (path.endsWith(".csv"))   mediaType = "text/csv";
  else if (path.endsWith(".js"))    mediaType = "application/javascript";
  else if (path.endsWith(".png"))   mediaType = "image/png";

  String minPath = path.substring(0, path.lastIndexOf("."))+".min"+path.substring(path.lastIndexOf("."), path.length());

  if (_sdCard.fileExists(minPath)){
    path=minPath;
  }


  if (_sdCard.fileExists(path)) {
    String file = _sdCard.readFile(path);
    if (file != "") {
      _server.send(200, mediaType, file);
      return true;
    }
  }

  _server.send(404, "text/plain", "File not Found.");
  return false;
}

void GZ_WebServer::handleRoot(){
  if (!_sdCard.begin(SD_CS)) {
    DEBUG_PRINTLN("SD Card could not begin");
    _server.send(404, "text/plain", "SD Card Not Found.");
    return;
  }

  if ( !loadFromMemoryCard(_server.uri()) ) {
    _server.send(404, "Page Not Found.");
  }
}


void GZ_WebServer::handleJSON(){
  DEBUG_PRINTLN("JSON Requested.");

  String JSON = "";
  JSON += "{";
  JSON += "\"temperature\":" + String(_airSensor->getTemperature())     +",";
  JSON += "\"pressure\":"    + String(_airSensor->getPressure())        +",";
  JSON += "\"humidity\":"    + String(_airSensor->getHumidity())        +",";
  JSON += "\"gas\":"         + String(_airSensor->getGas_resistance())  +",";
  JSON += "\"csventry\":\""  + String(get_CSV_data())                   +"\"";
  JSON += "}";

  DEBUG_PRINTLN("JSON: "+JSON);
  _server.send(200,"application/json",JSON);
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

String GZ_WebServer::get_CSV_data(){
  String gpsInfos = _gps->getInfos();
  String bmeInfos = _airSensor->getInfos();
  if(bmeInfos != "0" and gpsInfos != "0")
    return gpsInfos + "," + bmeInfos;
  else
    return "0";
}

void GZ_WebServer::write_CSV_entry(){
  _sdCard.writeFile(String(ROOT_PATH)+"/data.csv", get_CSV_data());
}
