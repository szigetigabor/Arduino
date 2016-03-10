#ifndef SCT013
#define SCT013

#define PIN 0
#include "EmonLib.h"  
EnergyMonitor emon1;             // Create an instance


void initSCT()
{
  emon1.voltage(PIN, 234.26, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon1.current(PIN, 111.1);       // Current: input pin, calibration.
}


float getCurrentPower() {
  emon1.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
  return emon1.apparentPower;
}
#endif
