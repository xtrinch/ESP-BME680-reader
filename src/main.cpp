#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#ifdef ESP32
  #include <HTTPClient.h>
  #include <WiFi.h>
#elif ESP8266
  #include <ESP8266HTTPClient.h>
  #include <ESP8266WiFi.h>
#endif

using namespace std;

#define SEALEVELPRESSURE_HPA (1013.25)

// sleep defines
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  300       /* Time ESP32 will go to sleep (in seconds) - 5min */

// macro to string expansion for env variables
#define xstr(s) strs(s)
#define strs(s) #s

Adafruit_BME680 bme; // I2C
HTTPClient http;

void setupWiFi() {
  WiFi.begin(xstr(WIFI_SSID), xstr(WIFI_PASSWORD));

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.println("Connected to WiFi");
}

void setupBME680() {
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    return;
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}

void goToSleep() {
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
  " Seconds");

  #ifdef ESP32
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
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
  ESP.deepSleep(TIME_TO_SLEEP * uS_TO_S_FACTOR, WAKE_RFCAL);
  #endif

  Serial.println("This will never be printed");
}

String getJsonPayload(Adafruit_BME680 bme) {
  float temperature = bme.temperature;
  // Serial.print("Temperature = ");
  // Serial.print(temperature);
  // Serial.println(" *C");

  float pressure = bme.pressure / 100.0;
  // Serial.print("Pressure = ");
  // Serial.print(pressure);
  // Serial.println(" hPa");

  float humidity = bme.humidity;
  // Serial.print("Humidity = ");
  // Serial.print(humidity);
  // Serial.println(" %");

  float gas_resistance = bme.gas_resistance / 1000.0;
  // Serial.print("Gas = ");
  // Serial.print(gas_resistance);
  // Serial.println(" KOhms");

  float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  // Serial.print("Approx. Altitude = ");
  // Serial.print(altitude);
  // Serial.println(" m");

  String jsonPayload = "{" 
      "\"measurements\":["
        "{"
            "\"measurement\": " + String(humidity) + ","
            "\"measurementType\": \"humidity\""
        "},"
        "{"
            "\"measurement\": " + String(temperature) + ","
            "\"measurementType\": \"temperature\""
        "},"
        "{"
            "\"measurement\": " + String(gas_resistance) + ","
            "\"measurementType\": \"gas\""
        "},"
        "{"
            "\"measurement\": " + String(altitude) + ","
            "\"measurementType\": \"altitude\""
        "},"
        "{"
            "\"measurement\": " + String(pressure) + ","
            "\"measurementType\": \"pressure\""
        "}"
      "]"
  "}";

  return jsonPayload;
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  setupBME680();

  setupWiFi();

  http.begin("http://" + String(xstr(SENSOR_DASHBOARD_URL)) + "/api/measurements/multi");

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");

    goToSleep();
    return;
  }

  if(WiFi.status() != WL_CONNECTED) {
    Serial.print("WIFI disconnected.");
    delay(5000);
    return;
  }
  
  String jsonPayload = getJsonPayload(bme);
  // Serial.print(jsonPayload);

  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept", "application/json");
  http.addHeader("Authorization", xstr(SENSOR_ACCESS_TOKEN));
  int httpResponseCode = http.POST(jsonPayload);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    //Serial.println(payload);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  goToSleep();
}