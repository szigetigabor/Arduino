#ifndef EEPROM_CONTROL
#define EEPROM_CONTROL

#include <EEPROM.h>
#include "Temp_config.h"

#define BACKLIGHT 20 
#define SLEEP 2 
/*
 * EEPROM map:
 *  0. reset
 *  1. mode
 *  2. sleep time
 *  3-x. room infos (41 bytes each info)
 *  x-y. extra temperature sensors (28 bytes each info)
 *  y-999. empty space
 *  1000. scheduling day hour
 *  1001. scheduling day minute
 *  1002. scheduling night hour
 *  1003. scheduling night minute
 *  1004-4096. empty space
 */
#define BASE_OFFSET 3
#define SCHEDULING_OFFSET 1000

int getSensorAddress(int index) {
  int eeAddress = BASE_OFFSET;
  if ( index < ROOMS ) {
    eeAddress += index*sizeof(TempSensorData);
  } else if (index < ALL_SENSORS) {
    eeAddress += ROOMS*sizeof(TempSensorData);
    eeAddress += (index-ROOMS)*sizeof(TempSensor);
  } else if (index == ALL_SENSORS) {
    eeAddress += ROOMS*sizeof(TempSensorData);
    eeAddress += (index-ROOMS+1)*sizeof(TempSensor);
  }
  return eeAddress;
}

#endif

