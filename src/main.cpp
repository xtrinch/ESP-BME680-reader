#include "main.h"

using namespace std;

// sleep defines
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */

void goToSleep() {
  Serial.println("Setup ESP32 to sleep for every " + String(CFG_SLEEP_SECONDS) +
  " Seconds");

  #ifdef ESP32
  esp_sleep_enable_timer_wakeup(CFG_SLEEP_SECONDS * uS_TO_S_FACTOR);
  // shut practically everything off
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL, ESP_PD_OPTION_OFF);
  #endif

  delay(1000);
  Serial.flush();

  #ifdef ESP32
  esp_deep_sleep_start();
  #elif ESP8266
  ESP.deepSleep(CFG_SLEEP_SECONDS * uS_TO_S_FACTOR, WAKE_RFCAL);
  #endif

  Serial.println("This will never be printed");
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  setupEEPROM();

  if (!setupAP()) {
    goToSleep();
  }

  listenForCredentials();

  cleanupAP();
  
  if (!setupWiFi()) {
    goToSleep();
  }

  if (!setupBME680()) {
    goToSleep();
  };
}

void loop() {
  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");

    goToSleep();
    return;
  }

  if(WiFi.status() != WL_CONNECTED) {
    Serial.print("WIFI disconnected.");
    return;
  }
  
  String jsonPayload = getJsonPayload(bme);
  // Serial.print(jsonPayload);

  sendMeasurement(jsonPayload);

  goToSleep();
}