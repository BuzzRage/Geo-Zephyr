#include "airQHandler.h"

GZ_AirSensor::GZ_AirSensor(){
  //_airSensor = new Adafruit_BME680();
}

float GZ_AirSensor::getTemperature(){
  return _airSensor.readTemperature();
}

uint32_t GZ_AirSensor::getPressure(){
  return _airSensor.readPressure();
}

float GZ_AirSensor::getHumidity(){
  return _airSensor.readHumidity();
}


uint32_t GZ_AirSensor::getGas_resistance(){
  return _airSensor.readGas();
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

void GZ_AirSensor::printReadings(){
  if (! _airSensor.performReading()) {
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
  if (! _airSensor.performReading()) {
   return "0";
  }

  String dataString = "";
  dataString += String(_airSensor.temperature) + ",";
  dataString += String(_airSensor.pressure / 100.0) + ",";
  dataString += String(_airSensor.humidity) + ",";
  dataString += String(_airSensor.gas_resistance / 1000.0) + ",";
  dataString += String(_airSensor.readAltitude(SEALEVELPRESSURE_HPA));
  return dataString;
}
