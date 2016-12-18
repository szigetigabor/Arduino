#ifndef SCT013
#define SCT013

#define PIN 0
#define POWERLIMIT 30
#include "EmonLib.h"  
EnergyMonitor emon1;             // Create an instance


void initSCT()
{
  emon1.voltage(PIN, 234.26, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon1.current(PIN, 111.1);       // Current: input pin, calibration.
}

// Get current power consuption
float getCurrentPower() {
  return emon1.calcIrms(1480)*0.23;
}

bool boilerRunningCheck() {
  bool retval = false;
  if ( getCurrentPower() >= POWERLIMIT ) {
    retval = true;
  }
  return retval;
}
#endif
