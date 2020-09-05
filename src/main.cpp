#include "main.h"

using namespace std;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  setupEEPROM();

  setupButton();

  if (!configSaved()) {
    if (!setupAP()) {
      goToSleep();
    }

    listenForConfig();
    cleanupAP();
  }

  // do not do anything if button is pressed
  if (digitalRead(BTN_PIN) == BTN_PRESSED_STATE) {
    Serial.println("Button pressed upon startup, skipping WiFi setup");
    return;
  }

  if (!setupWiFi()) {
    goToSleep();
  }

  if (!setupBME680()) {
    goToSleep();
  };

  if (!bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    goToSleep();
    return;
  }

  if(WiFi.status() != WL_CONNECTED) {
    Serial.print("WIFI disconnected.");
    goToSleep();
    return;
  }
  
  String jsonPayload = getJsonPayload(bme);
  // Serial.print(jsonPayload);

  sendMeasurement(jsonPayload);
}

void loop() {
  /* do nothing in loop except check button, 
    esp will be in deep sleep in between measurements which will make setup re-run */

  if (checkButtonPressed()) {
      // do not execute anything else when button is pressed
    return;
  }

  goToSleep();
}