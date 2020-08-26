#include "bme680.h"

Adafruit_BME680 bme; 

bool setupBME680() {
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    return false;
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

  return true;
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