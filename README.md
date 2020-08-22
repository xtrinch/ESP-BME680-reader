# sensor-dashboard-ESP32-BME680-reader

Sends periodic `BME680` humidity, temperature, gas, pressure and altitude data from `esp32/esp8266 development board` to sensor dashboard backend.

Built with PlatformIO.

## Setup

1. Have a sensor dashboard backend running (see https://github.com/xtrinch/sensor-dashboard-nestjs-backend)
2. Copy `import_env.example.py` to `import_env.py` and define you wifi SSID, passphrase, sensor access token & sensor dashboard url.