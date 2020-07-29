#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <HTTPClient.h>
#include <WiFi.h>

using namespace std;

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

// macro to string expansion
#define xstr(s) strs(s)
#define strs(s) #s

Adafruit_BME680 bme; // I2C

HTTPClient http;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println(F("BME680 test"));

  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

  WiFi.begin(xstr(WIFI_SSID), xstr(WIFI_PASSWORD));

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.println("Connected to WiFi");

  http.begin("http://" + String(xstr(SENSOR_DASHBOARD_URL)) + "/measurements/multi");
}

void loop() {
  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }

  float temperature = bme.temperature;
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" *C");

  float pressure = bme.pressure / 100.0;
  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" hPa");

  float humidity = bme.humidity;
  Serial.print("Humidity = ");
  Serial.print(humidity);
  Serial.println(" %");

  float gas_resistance = bme.gas_resistance / 1000.0;
  Serial.print("Gas = ");
  Serial.print(gas_resistance);
  Serial.println(" KOhms");

  float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  Serial.print("Approx. Altitude = ");
  Serial.print(altitude);
  Serial.println(" m");

  if(WiFi.status() != WL_CONNECTED) {
    Serial.print("WIFI disconnected.");
    delay(5000);
    return;
  }

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

  Serial.print(jsonPayload);

  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept", "application/json");
  http.addHeader("Authorization", xstr(SENSOR_ACCESS_TOKEN));
  int httpResponseCode = http.POST(jsonPayload);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  Serial.println();
  delay(60000);
}