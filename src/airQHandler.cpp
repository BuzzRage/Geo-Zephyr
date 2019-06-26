#include "airQHandler.h"

GZ_AirSensor::GZ_AirSensor(){
  //_airSensor = new Adafruit_BME680();
  gas_reference = 250000;
}

float GZ_AirSensor::getTemperature(){
  return _airSensor.readTemperature();
}

float GZ_AirSensor::getPressure(){
  return float(_airSensor.readPressure()/100.0);
}

float GZ_AirSensor::getHumidity(){
  return _airSensor.readHumidity();
}


float GZ_AirSensor::getGas_resistance(){
  return float(_airSensor.readGas()/1000.0);
}

float GZ_AirSensor::getAirQScore(){
  // Calcul de la contribution de l'humidité dans l'index IAQ
  float current_humidity = getHumidity();
  if (current_humidity >= 38 && current_humidity <= 42)
    hum_score = 0.25*100;
  else{
    if(current_humidity < 38)  hum_score = 0.25/HUM_REF*current_humidity*100;
    else                       hum_score = ((-0.25/(100-HUM_REF)*current_humidity)+0.416666)*100;
  }

  // Calcul de la contribution du gaz dans l'index IAQ
  float gas_lower_limit = 5000;   // Bad air quality limit
  float gas_upper_limit = 50000;  // Good air quality limit
  if (gas_reference > gas_upper_limit)    gas_reference = gas_upper_limit;
  if (gas_reference < gas_lower_limit)    gas_reference = gas_lower_limit;

  gas_score = (0.75/(gas_upper_limit-gas_lower_limit)*gas_reference - (gas_lower_limit*(0.75/(gas_upper_limit-gas_lower_limit))))*100;

  airQ_score = hum_score + gas_score;
  return airQ_score;
}

String GZ_AirSensor::getIAQ(float score){
  String IAQ_text = "";
  score = (100-score)*5;
  if      (score >= 301)                  IAQ_text += "Hazardous";
  else if (score >= 201 && score <= 300 ) IAQ_text += "Very Unhealthy";
  else if (score >= 176 && score <= 200 ) IAQ_text += "Unhealthy";
  else if (score >= 151 && score <= 175 ) IAQ_text += "Unhealthy for Sensitive Groups";
  else if (score >=  51 && score <= 150 ) IAQ_text += "Moderate";
  else if (score >=  00 && score <=  50 ) IAQ_text += "Good";
  return IAQ_text;
}

void GZ_AirSensor::init(){
  DEBUG_PRINTLN("Air quality sensor initialization.");

  if (!_airSensor.begin(I2C_ADDR)) {
    DEBUG_PRINTLN("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }
  // Set up oversampling and filter initialization
  _airSensor.setTemperatureOversampling(BME680_OS_8X);
  _airSensor.setHumidityOversampling(BME680_OS_2X);
  _airSensor.setPressureOversampling(BME680_OS_4X);
  _airSensor.setIIRFilterSize(BME680_FILTER_SIZE_3);
  _airSensor.setGasHeater(320, 150); // 320*C for 150 ms
}

int GZ_AirSensor::performReading(){
  if (! _airSensor.performReading()) {
   return 0;
  }
  return 1;
}

void GZ_AirSensor::printReadings(){
  if (!performReading()) {
   DEBUG_PRINTLN("Failed to perform reading :(");
   return;
 }
 DEBUG_PRINT("Temperature = ");
 DEBUG_PRINT(_airSensor.temperature);
 DEBUG_PRINTLN(" *C");

 DEBUG_PRINT("Pressure = ");
 DEBUG_PRINT(_airSensor.pressure / 100.0);
 DEBUG_PRINTLN(" hPa");

 DEBUG_PRINT("Humidity = ");
 DEBUG_PRINT(_airSensor.humidity);
 DEBUG_PRINTLN(" %");

 DEBUG_PRINT("Gas = ");
 DEBUG_PRINT(_airSensor.gas_resistance / 1000.0);
 DEBUG_PRINTLN(" KOhms");

 DEBUG_PRINT("Approx. Altitude = ");
 DEBUG_PRINT(_airSensor.readAltitude(SEALEVELPRESSURE_HPA));
 DEBUG_PRINTLN(" m");

}

String GZ_AirSensor::getInfos(){
  if (!performReading()) {
   return "0";
  }

  String dataString = "";
  dataString += String(_airSensor.temperature) + ",";
  dataString += String(_airSensor.pressure / 100.0) + ",";
  dataString += String(_airSensor.humidity) + ",";
  dataString += String(_airSensor.gas_resistance / 1000.0) + ",";
  dataString += String(getAirQScore()) + ",";
  dataString += String(getIAQ(airQ_score)) + ",";
  dataString += String(_airSensor.readAltitude(SEALEVELPRESSURE_HPA));
  return dataString;
}
