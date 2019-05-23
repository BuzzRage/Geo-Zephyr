#include "airQHandler.h"

GZ_AirSensor::GZ_AirSensor(){
  //_airSensor = new Adafruit_BME680();
}

float GZ_AirSensor::getTemperature(){
  return _airSensor.temperature;
}

uint32_t GZ_AirSensor::getPressure(){
  return _airSensor.pressure;
}

float GZ_AirSensor::getHumidity(){
  return _airSensor.humidity;
}


uint32_t GZ_AirSensor::getGas_resistance(){
  return _airSensor.gas_resistance;
}

void GZ_AirSensor::init(){
  Serial.println("Air quality sensor initialization.");

  if (!_airSensor.begin(I2C_ADDR)) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }
  // Set up oversampling and filter initialization
  _airSensor.setTemperatureOversampling(BME680_OS_8X);
  _airSensor.setHumidityOversampling(BME680_OS_2X);
  _airSensor.setPressureOversampling(BME680_OS_4X);
  _airSensor.setIIRFilterSize(BME680_FILTER_SIZE_3);
  _airSensor.setGasHeater(320, 150); // 320*C for 150 ms
}

void GZ_AirSensor::test(){
  if (! _airSensor.performReading()) {
   Serial.println("Failed to perform reading :(");
   return;
 }
 Serial.print("Temperature = ");
 Serial.print(_airSensor.temperature);
 Serial.println(" *C");

 Serial.print("Pressure = ");
 Serial.print(_airSensor.pressure / 100.0);
 Serial.println(" hPa");

 Serial.print("Humidity = ");
 Serial.print(_airSensor.humidity);
 Serial.println(" %");

 Serial.print("Gas = ");
 Serial.print(_airSensor.gas_resistance / 1000.0);
 Serial.println(" KOhms");

 Serial.print("Approx. Altitude = ");
 Serial.print(_airSensor.readAltitude(SEALEVELPRESSURE_HPA));
 Serial.println(" m");

 Serial.println();
}
