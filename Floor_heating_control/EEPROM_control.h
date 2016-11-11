#ifndef EEPROM_CONTROL
#define EEPROM_CONTROL

#include <EEPROM.h>
#include "Temp_config.h"

#define BACKLIGHT 20 
#define SLEEP 2 
/*
 * EEPROM map:
 *  0. reset
 *  1. blacklight
 *  2. sleep time
 *  3-x. room infos (39 bytes each info)
 *  x-y. extra temperature sensors
 */
#define BASE_OFFSET 3

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

