#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include "common.h"
#include "eeprom-handler.h"
#include "bme680.h"
#include "access-point.h"
#include "station.h"

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define BTN_PIN 13
#define BTN_PRESS_LENGTH 2000 //ms
#define BTN_SAMPLE_INTERVAL 25
#define BTN_RESET_CYCLES 80    // 80 * 25 = 2000 ms
#define BTN_PRESSED_STATE LOW
#define BTN_RELEASED_STATE HIGH

// config defines
#define CFG_SLEEP_SECONDS atoi(xstr(SLEEP_SECONDS))

#endif