#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <string.h>
#include <stdarg.h>
#include <Arduino.h>

#define ARDBUFFER 32

// macro to string expansion for env variables
#define xstr(s) strs(s)
#define strs(s) #s

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define mS_TO_S_FACTOR 1000  /* Conversion factor for mili seconds to seconds */

void ardprintf(const char *str, ...);
void append(char* s, char c);

#endif