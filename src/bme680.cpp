#include "bme680.h"

Adafruit_BME680 bme; 

bool setupBME680() {
  // i2c address 0x76
  if (!bme.begin(CFG_I2C_ADDRESS)) {
    ardprintf("BME680: Could not find a valid BME680 sensor, check wiring!");
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

bool getJsonPayload(char * buf, Adafruit_BME680 bme) {
  float temperature = bme.temperature;
  // Serial.print("Temperature = ");
  // Serial.print(temperature);
  // ardprintf(" *C");

  float pressure = bme.pressure / 100.0;
  // Serial.print("Pressure = ");
  // Serial.print(pressure);
  // ardprintf(" hPa");

  float humidity = bme.humidity;
  // Serial.print("Humidity = ");
  // Serial.print(humidity);
  // ardprintf(" %");

  float gas_resistance = bme.gas_resistance / 1000.0;
  // Serial.print("Gas = ");
  // Serial.print(gas_resistance);
  // ardprintf(" KOhms");

  float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  // Serial.print("Approx. Altitude = ");
  // Serial.print(altitude);
  // ardprintf(" m");

  snprintf(buf, 500, "{" 
      "\"measurements\":["
        "{"
            "\"measurement\": %f,"
            "\"measurementType\": \"humidity\""
        "},"
        "{"
            "\"measurement\": %f,"
            "\"measurementType\": \"temperature\""
        "},"
        "{"
            "\"measurement\": %f,"
            "\"measurementType\": \"gas\""
        "},"
        "{"
            "\"measurement\": %f,"
            "\"measurementType\": \"altitude\""
        "},"
        "{"
            "\"measurement\": %f,"
            "\"measurementType\": \"pressure\""
        "}"
      "]"
  "}", humidity, temperature, gas_resistance, altitude, pressure);

  return true;
}