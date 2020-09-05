#ifndef SLEEP_H_INCLUDED
#define SLEEP_H_INCLUDED

#include <Arduino.h>
#include "common.h"

void goToSleep();

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */

// config defines
#define CFG_SLEEP_SECONDS atoi(xstr(SLEEP_SECONDS))

#endif