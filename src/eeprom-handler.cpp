
#include "eeprom-handler.h"

void setupEEPROM() {
  EEPROM.begin(512);  //Initialize EEPROM
}

int getEEPROMAddress(String name) {
  if (strcmp(name.c_str(), "ssid") == 0) {
    return WIFI_SSID_ADDRESS;
  } else if (strcmp(name.c_str(), "password") == 0) {
    return WIFI_PASSWORD_ADDRESS;
  } else if (strcmp(name.c_str(), "magic_string") == 0) {
    return MAGIC_STRING_ADDRESS;
  } else if (strcmp(name.c_str(), "access_token") == 0) {
    return SENSOR_ACCESS_TOKEN_ADDRESS;
  }

  return -1;
}

String readFromEEPROM(String name) {
  int addr = getEEPROMAddress(name);
  if (addr < 0) {
    Serial.println("Could not read unknown value " + name);
    return "";
  }
  char value[32];

  for(int i=0; i < 32; i++) {
    char ch = (char)EEPROM.read(addr+i);
    value[i] = ch;

    if (ch == '\0') {
      break;
    }
  }

  Serial.println("Read from EEPROM: " + (name) + " " + (value));
  return String(value);
}

bool configSaved() {
  String magicString = readFromEEPROM("magic_string");
  if (strcmp(magicString.c_str(), xstr(MAGIC_STRING)) != 0) {
    Serial.println("No WiFi credentials saved.");
    return false;
  }

  return true;
}

Config readConfig() {
  Config credentials;

  String ssid = readFromEEPROM("ssid");
  String password = readFromEEPROM("password");

  credentials.ssid = ssid;
  credentials.password = password;

  return credentials;
}

bool clearConfig() {
  Serial.println("Clearing Wi-Fi credentials");
  return saveToEEPROM("magic_string", "");
}

bool saveConfig(String ssid, String password, String sensorAccessToken) {
  if (!ssid || ssid.length() == 0 || !password || password.length() == 0) {
    return false;
  }
  
  bool retVal = true;

  if (
    !saveToEEPROM("ssid", ssid) ||
    !saveToEEPROM("password", password) ||
    !saveToEEPROM("access_token", sensorAccessToken) ||
    !saveToEEPROM("magic_string", xstr(MAGIC_STRING))
  ) {
    retVal = false;
  }

  return retVal;
}

bool saveToEEPROM(String name, String value) { 
  int addr = getEEPROMAddress(name);

  if (addr < 0) {
    Serial.println("Cannot save: unknown value " + value + " for " + name);
    return false;
  }
 
  for(int i=0; i < (int)value.length(); i++) {
    EEPROM.write(addr+i, value[i]);
  }
  EEPROM.write(addr+value.length(),'\0');

  bool success = EEPROM.commit(); 

  if (!success) {
    Serial.println("Could not save to EEPROM");
    return false;
  }

  Serial.println("Saved to EEPROM: " + (name) + " " + (value));
  return true;
}