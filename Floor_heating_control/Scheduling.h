#ifndef SCHEDULING
#define SCHEDULING

#include "EEPROM_control.h"

#define DAY_HOUR   6
#define DAY_MINUTE 30
#define NIGHT_HOUR   20
#define NIGHT_MINUTE 30

byte day_hour, day_minute, night_hour, night_minute;

void initScheduling() {
  // get values from EEPROM or use default values
  day_hour = getDayHour();
  if (day_hour == 0 || day_hour > 23) {
    day_hour = DAY_HOUR;
  }
  
  day_minute = getDayMinute();
  if (day_minute == 0 || day_minute > 59) {
    day_minute = DAY_MINUTE;
  }
  
  night_hour = getNightHour();
  if (night_hour == 0 || night_hour > 23) {
    night_hour = NIGHT_HOUR;
  }
  
  night_minute = getNightMinute();
  if (night_minute == 0 || night_minute > 59) {
    night_minute = NIGHT_MINUTE;
  }
}

bool isDay() {
  bool retval = false;
  int cHour = (int)tmpRTC.tHour;
  int cMinute = (int)tmpRTC.tMinute;
  if ( ( cHour == day_hour   && cMinute >= day_minute ) ||
       ( cHour > day_hour    && cHour < night_hour ) ||
       ( cHour == night_hour && cMinute < night_minute ) ) {
    retval = true;
  }
  return retval;
}

bool isNight() {
  return !isDay();
}

#endif
