
#include "eeprom-handler.h"

using namespace std;

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

WiFiCredentials readWiFiCredentials() {
  struct WiFiCredentials credentials;
  credentials.ssid = readFromEEPROM("ssid");
  credentials.password = readFromEEPROM("password");

  return credentials;
}

bool saveWiFiCredentials(String ssid, String password) {
  if (!ssid || ssid.length() == 0 || !password || password.length() == 0) {
    return false;
  }
  
  bool retVal = true;

  if (
    !saveToEEPROM("ssid", ssid) ||
    !saveToEEPROM("password", password) ||
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