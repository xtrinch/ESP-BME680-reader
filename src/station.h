#ifndef STATION_H_INCLUDED
#define STATION_H_INCLUDED

#include <Arduino.h>
#ifdef ESP32
  #include <HTTPClient.h>
  #include <WiFi.h>
#elif ESP8266
  #include <ESP8266HTTPClient.h>
  #include <ESP8266WiFi.h>
#endif

#include <Arduino.h>
#include "common.h"
#include "eeprom-handler.h"

#define CFG_SENSOR_DASHBOARD_URL xstr(SENSOR_DASHBOARD_URL)
#define CFG_SENSOR_ACCESS_TOKEN xstr(SENSOR_ACCESS_TOKEN)

bool setupWiFi();
bool sendMeasurement(String jsonPayload);

#endif