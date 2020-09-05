#ifndef SLEEP_H_INCLUDED
#define SLEEP_H_INCLUDED

#include <Arduino.h>
#include "common.h"

void goToSleep();

// config defines
#define CFG_SLEEP_SECONDS atoi(xstr(SLEEP_SECONDS))

#endif