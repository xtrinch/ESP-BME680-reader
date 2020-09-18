#ifndef BME680_H_INCLUDED
#define BME680_H_INCLUDED

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include "common.h"

#define SEALEVELPRESSURE_HPA (1013.25)

extern Adafruit_BME680 bme; // I2C

bool setupBME680();
bool getJsonPayload(char * buf, Adafruit_BME680 bme);

#endif