# sensor-dashboard-ESP-BME680-reader

Sends periodic `BME680` humidity, temperature, gas, pressure and altitude data from `esp32/esp8266 development board` to sensor dashboard backend.

Built with PlatformIO.

See also [backend](https://github.com/xtrinch/sensor-dashboard-nestjs-backend) and [frontend](https://github.com/xtrinch/sensor-dashboard-react-frontend), or [see it live](http://sensor-dashboard.trina.si/) with some sensor data from my living room.

## Setup

1. Have a sensor dashboard backend running (see https://github.com/xtrinch/sensor-dashboard-nestjs-backend)
2. Copy `import_env.example.py` to `import_env.py` and define you wifi SSID, passphrase, sensor access token & sensor dashboard url.
