#ifndef GLOBAL_H
#define GLOBAL_H

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>

#define DEBUG_DISPLAY
#ifdef DEBUG_DISPLAY
	#define DEBUG_PRINTLN(a) 	Serial.println(a)
	#define DEBUG_PRINT(a) 		Serial.print(a)
	#define DEBUG_WRITE(a) 		Serial.write(a)
#else
	#define DEBUG_PRINTLN(a)
	#define DEBUG_PRINT(a)
	#define DEBUG_WRITE(a)
#endif

#define SERIAL_SPEED 		      9600
#define WEBSERVER_PORT				80

// Defines peripheral pins
#define BME_VCC               3V3   // 3.3v
#define BME_GND               GND   // GND
#define BME_SDA               SDA   // 21
#define BME_SCL               SCL   // 22
#define I2C_ADDR							0x76  // I2C Address for Sparkun ESP32 Thing

#define SEALEVELPRESSURE_HPA  1013.25

void initBme();
void testBME();

#endif
