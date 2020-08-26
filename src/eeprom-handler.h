#ifndef EEPROM_HANDLER_H_INCLUDED
#define EEPROM_HANDLER_H_INCLUDED

#include <Arduino.h>
#include <EEPROM.h>
#include "common.h"

#define MAGIC_STRING_ADDRESS 0x00
#define STATE_MACHINE_ADDRESS 0x32
#define WIFI_SSID_ADDRESS 0x64
#define WIFI_PASSWORD_ADDRESS 0x96

#define MAGIC_STRING "12345678"

#define CFG_WIFI_SSID xstr(WIFI_SSID)
#define CFG_WIFI_PASSWORD xstr(WIFI_PASSWORD)

struct WiFiCredentials {
  String ssid;
  String password;
};

void setupEEPROM();
bool saveWiFiCredentials(String ssid, String password);
bool saveToEEPROM(String name, String value);
struct WiFiCredentials readWiFiCredentials();

#endif