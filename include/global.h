#ifndef GLOBAL_H
#define GLOBAL_H

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
//#define WIFI_ENABLED
#ifdef WIFI_ENABLED
	#define WEBSERVER_PORT				80
#endif
/* ---------- Defines peripheral pins ------------------------ */
// Air Quality control
#define BME_VCC               3V3   // 3.3v
#define BME_GND               GND   // GND
#define BME_SDA               SDA   // 21
#define BME_SCL               SCL   // 22
#define I2C_ADDR							0x76  // I2C Address for Sparkun ESP32 Thing

//GPS
#define GPS_VCC								3V3
#define GPS_GND								GND
#define GPS_TX								16
#define GPS_RX								17


#define SEALEVELPRESSURE_HPA  1013.25


#endif
