#ifndef EEPROM_HANDLER_H_INCLUDED
#define EEPROM_HANDLER_H_INCLUDED

#include <Arduino.h>
#include <EEPROM.h>
#include "common.h"

#define MAGIC_STRING_ADDRESS 0
#define STATE_MACHINE_ADDRESS 32
#define WIFI_SSID_ADDRESS 64
#define WIFI_PASSWORD_ADDRESS 96
#define SENSOR_ACCESS_TOKEN_ADDRESS 128 // length 34 chars

#define MAGIC_STRING "12345678"

#define CFG_WIFI_SSID xstr(WIFI_SSID)
#define CFG_WIFI_PASSWORD xstr(WIFI_PASSWORD)

using namespace std;

typedef struct Config_t {
  String ssid;
  String password;
} Config;

void setupEEPROM();
bool saveConfig(String ssid, String password, String sensorAccessToken);
bool saveToEEPROM(String name, String value);
Config readConfig();
bool configSaved();
bool clearConfig();
String readFromEEPROM(String name);

#endif