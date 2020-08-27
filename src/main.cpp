#include "main.h"

using namespace std;

// sleep defines
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */

void ICACHE_RAM_ATTR wakeUp(void)
{
  // do nothing
}

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

int val;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  setupEEPROM();

  // pin is pulled low by the button when active
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // // wake up when button is pulled low
  // attachInterrupt(BUTTON_PIN, wakeUp, FALLING);

  if (!WiFiCredentialsSaved()) {
    if (!setupAP()) {
      goToSleep();
    }

    listenForCredentials();
    cleanupAP();
  }

  val = digitalRead(BUTTON_PIN); // check the state of the button
  if (val != LOW) {
    if (!setupWiFi()) {
      goToSleep();
    }

    if (!setupBME680()) {
      goToSleep();
    };
  }
}

int prevVal;
long keyPressCount = 0;
const unsigned long keySampleIntervalMs = 25;
unsigned long keyPrevMillis = 0;
byte longKeyPressCountMax = 80;    // 80 * 25 = 2000 ms

void loop() {
  val = digitalRead(BUTTON_PIN); // check the state of the button

  if (millis() - keyPrevMillis >= keySampleIntervalMs) {
    keyPrevMillis = millis();

    if(val == LOW && prevVal == HIGH) // if button is pressed, turn LED on
    {
      Serial.println("Key press");
      keyPressCount = 0;
    }
    else if (val == HIGH && prevVal == LOW)
    {
      Serial.println("Key release");
    } else if (val == LOW) {
      keyPressCount ++;
    }

    if (keyPressCount >= longKeyPressCountMax) {
      Serial.println("Long key press");
      clearWiFiCredentials();
      // something very negative
      keyPressCount = -10*longKeyPressCountMax;
    }

    prevVal = val;
  }

  if (val == LOW) {
    // do not execute anything else when button is pressed
    return;
  }

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