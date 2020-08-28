#include "station.h"

HTTPClient http;

bool setupWiFi() {
  int wifiRetriesLeft = 50;

  Config credentials = readConfig();

  WiFi.begin(credentials.ssid.c_str(), credentials.password.c_str());

  while (WiFi.status() != WL_CONNECTED && wifiRetriesLeft > 0) {
    delay(500);
    Serial.println("Connecting to WiFi with SSID " + credentials.ssid + " and password " + credentials.password);
    wifiRetriesLeft -= 1;
  }

  if (wifiRetriesLeft <= 0 || WiFi.status() != WL_CONNECTED) {
    Serial.println("Could not connect to WiFi.");
    return false;
  }
  
  Serial.println("Connected to WiFi");

  http.begin("http://" + String(CFG_SENSOR_DASHBOARD_URL) + "/api/measurements/multi");

  return true;
}

bool sendMeasurement(String jsonPayload) {
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept", "application/json");
  http.addHeader("Authorization", readFromEEPROM("access_token"));
  int httpResponseCode = http.POST(jsonPayload);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    //Serial.println(payload);
    return true;
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
    return false;
  }

  return false;
}