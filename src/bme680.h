#ifndef BME680_H_INCLUDED
#define BME680_H_INCLUDED

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define SEALEVELPRESSURE_HPA (1013.25)

extern Adafruit_BME680 bme; // I2C

bool setupBME680();
String getJsonPayload(Adafruit_BME680 bme);

#endif