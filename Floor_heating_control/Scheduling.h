#include "RTC_control.h"

#define DAY_HOUR   6
#define DAY_MINUTE 30
#define NIGHT_HOUR   20
#define NIGHT_MINUTE 30

bool isDay() {
  bool retval = false;
  int Hour = (int)tmpRTC.tHour;
  int Minute = (int)tmpRTC.tMinute;
  if ( ( Hour == DAY_HOUR && Minute >= DAY_MINUTE ) ||
       ( Hour > DAY_HOUR && Hour < NIGHT_HOUR ) ||
       ( Hour == NIGHT_HOUR && Minute < NIGHT_MINUTE ) ) {
    retval = true;
  }
  return retval;
}

bool isNight() {
  return !isDay();
}

