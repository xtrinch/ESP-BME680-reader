#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include "common.h"
#include "eeprom-handler.h"
#include "bme680.h"
#include "access-point.h"
#include "station.h"
#include "pushbutton.h"

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */

// config defines
#define CFG_SLEEP_SECONDS atoi(xstr(SLEEP_SECONDS))

#endif