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
#define ESP_WEBSERVER_PORT		80

// Defines peripheral pins
#define BME_SCK               //To complete
#define BME_MISO              //To complete
#define BME_MOSI              //To complete
#define BME_CS                //To complete

#endif
