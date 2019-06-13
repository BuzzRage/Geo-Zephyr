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
  _server.on("/"                    ,std::bind(&GZ_WebServer::handleRoot, this));
  _server.on("/refreshData.js"      ,std::bind(&GZ_WebServer::handleJS,   this));
  _server.on("/sensors.json"        ,std::bind(&GZ_WebServer::handleJSON, this));
  _server.begin(_port);
}

void GZ_WebServer::run(){
    _server.handleClient();
}

void GZ_WebServer::handleRoot(){
  String str = "<!DOCTYPE html><html> \
  <head><meta http-equiv='Content-Type' content='text/html; charset=UTF-8' /> \
  <style>body { text-align: center; font-family: \"Trebuchet MS\", Arial;} \
            table { border-collapse: collapse; width:35%; margin-left:auto; margin-right:auto; } \
            th { padding: 12px; background-color: #af4300; color: white; } \
            tr { border: 1px solid #ddd; padding: 12px; } \
            tr:hover { background-color: #bcbcbc; } \
            td { border: none; padding: 12px; } </style></head> \
  <body> \
    <h1>GeoZéphyr</h1> \
    <table> \
      <tr> \
        <th>Mesure</th> \
        <th>Valeur</th> \
      </tr> \
      <tr> \
        <td>Température</td> \
        <td><div id='temperature'>"+String(_airSensor->getTemperature())+"</div>°C</td> \
      </tr> \
      <tr> \
        <td>Pression atmosphérique</td> \
        <td><div id='pressure'>"+String(_airSensor->getPressure()/100)+"</div>hPa</td> \
      </tr> \
      <tr> \
        <td>Taux d'humidité</td> \
        <td><div id='humidity'>"+String(_airSensor->getHumidity())+"</div>%</td> \
      </tr> \
      <tr> \
        <td>Résistance gaz</td> \
        <td><div id='gas'>"+String(_airSensor->getGas_resistance()/1000)+"</div>kOhms</td> \
      </tr> \
    </table> \
    <p>CSV header = Date, Time, Latitue, Longitude, Temperature, Pressure, Humidity, Gas Resistance</p> \
    <p>CSV entry = <div id='csventry'>"+get_CSV_data()+"</div></p> \
    <script> \
      function loadScript(){ \n \
  			var script = document.createElement('script'); \n \
  			script.type = 'text/javascript'; \n \
      	script.src = 'refreshData.js'; \n \
      	document.getElementsByTagName('head')[0].appendChild(script); \n \
      } \n \
      setTimeout('loadScript()',1000); \n \
    </script> \
  </body> \
  </html>";
  _server.send(200, "text/html", str);
}

void GZ_WebServer::handleJS(){
  String str = "var xmlHttp=createXmlHttpObject(); \
      var initDone = 0; \
      setTimeout('process()', 500); \
      function createXmlHttpObject(){ \
      	if(window.XMLHttpRequest){ \
      		xmlHttp=new XMLHttpRequest(); \
      	}else{ \
      		xmlHttp=new ActiveXObject('Microsoft.XMLHTTP'); \
      	} \
      	return xmlHttp; \
      } \
      function process(){ \
      	if(xmlHttp.readyState==0 || xmlHttp.readyState==4){ \
      		xmlHttp.open('GET','/sensors.json',true); \
      		xmlHttp.onreadystatechange=handleServerResponse; \
      		xmlHttp.send(null); \
      		initDone = 1; \
      	} \
      	if(initDone == 0) { \
      		setTimeout('process()',1000); \
      	} \
      } \
      function handleServerResponse(){ \
      	if(xmlHttp.readyState==4 && xmlHttp.status==200) \
      	{ \
      		ans =xmlHttp.responseXML; \
      		pressure 		= ans.getElementsByTagName('pressure')[0].firstChild.nodeValue; \
      		temperature = ans.getElementsByTagName('temperature')[0].firstChild.nodeValue; \
      		humidity		= ans.getElementsByTagName('humidity')[0].firstChild.nodeValue; \
      		gas				  = ans.getElementsByTagName('gas')[0].firstChild.nodeValue; \
      		document.getElementById('pressure').innerHTML = pressure; \
      		document.getElementById('temperature').innerHTML = temperature; \
      		document.getElementById('humidity').innerHTML = humidity; \
      		document.getElementById('gas').innerHTML = gas; \
      	} }";
  _server.send(200, "text/javascript", str);
}

void GZ_WebServer::handleJSON(){
  DEBUG_PRINTLN("JSON Requested.");

  String JSON = "";
  JSON += "{";
  JSON += "\"temperature\":" + String(_airSensor->getTemperature())     +",";
  JSON += "\"pressure\":"    + String(_airSensor->getPressure())        +",";
  JSON += "\"humidity\":"    + String(_airSensor->getHumidity())       +",";
  JSON += "\"gas\":"         + String(_airSensor->getGas_resistance()) +",";
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
