#include "station.h"

HTTPClient http;

bool setupWiFi() {
  int wifiRetriesLeft = 50;

  char ssid[60];
  readFromEEPROM(ssid, "ssid");
  char password[60];
  readFromEEPROM(password, "password");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED && wifiRetriesLeft > 0) {
    delay(500);
    ardprintf("Station: Connecting with SSID %s", ssid);
    ardprintf("Station: Connecting with password %s", password);

    wifiRetriesLeft -= 1;
  }

  if (wifiRetriesLeft <= 0 || WiFi.status() != WL_CONNECTED) {
    ardprintf("Station: Could not connect to WiFi.");
    return false;
  }
  
  ardprintf("Station: Connected to WiFi");

  http.begin("http://" + String(CFG_SENSOR_DASHBOARD_URL) + "/api/measurements/multi");

  return true;
}

bool sendMeasurement(const char * jsonPayload) {
  char accessToken[60];
  readFromEEPROM(accessToken, "access_token");

  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept", "application/json");
  http.addHeader("Authorization", accessToken);
  int httpResponseCode = http.POST(jsonPayload);

  if (httpResponseCode > 0) {
    ardprintf("Station: HTTP Response code: %d", httpResponseCode);
    // String payload = http.getString();
    // ardprintf(payload);
    return true;
  }
  else {
    ardprintf("Station: Error code: %d", httpResponseCode);
    return false;
  }

  return false;
}