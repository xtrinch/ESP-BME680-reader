#include "main.h"

void setup() {
  Serial.begin(9600);
  while (!Serial);

  setupEEPROM();

  // uncomment when you want to programmatically clear config
  // clearConfig();

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
    ardprintf("Button pressed upon startup, skipping WiFi setup");
    return;
  }

  if (!setupWiFi()) {
    goToSleep();
  }

  if (!setupBME680()) {
    goToSleep();
  };

  if (!bme.performReading()) {
    ardprintf("Failed to perform reading :(");
    goToSleep();
    return;
  }

  if(WiFi.status() != WL_CONNECTED) {
    ardprintf("WIFI disconnected.");
    goToSleep();
    return;
  }
  
  char jsonPayload[500];
  getJsonPayload(jsonPayload, bme);
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