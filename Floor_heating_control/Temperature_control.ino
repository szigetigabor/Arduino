#include "Temp_config.h"
#include "Relay.h"
#include <OneWire.h>
#define ONE_WIRE_BUS 19                  // DS18B20 is on P19

OneWire ds(ONE_WIRE_BUS);


float sensorRead(byte* _addr)
{
  byte j;
  byte present = 0;
  byte data[12];

  ds.reset();
  ds.select(_addr);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(_addr);    
  ds.write(0xBE);         // Read Scratchpad

  // Read and convert temperature
  for ( j = 0; j < 9; j++) {           // we need 9 bytes
    data[j] = ds.read();
  }

  int16_t raw = (data[1] << 8) | data[0];
  byte cfg = (data[4] & 0x60);
  // at lower res, the low bits are undefined, so let's zero them
  if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
  else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
  else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
  //// default is 12 bit resolution, 750 ms conversion time
  float celsius = (float)raw / 16.0;
  //int dec = (celsius - (int)celsius)*100;
  
  // CRC problem
  if ( celsius == 85.0 ) {   // or CRC = 21h
    celsius = 0.0;
  }

  return celsius;
}

void checkTemperature(float current, float required, byte relay) {
  bool output = false;
  if ( current < required) {
    output = true;
  }
  if ( relay_status[relay % 8] != output ) {
    setRelay(relay, output);
    relay_status[relay % 8] = output;
  }
}

void updateMainPump() {
  bool value = false;
  for (int i=0; i< ROOMS; i++) {
    value = value | relay_status[i];
  }
  value = value & boilerIsRunning;
  setRelay(MAIN_PUMP, value);
}

void initTemp() {
  last_checked = millis();
  last_checked_extra  = millis();
}

bool needTempCheck(unsigned long last_check, unsigned int waiting_time) {
  bool retval = false;
  _now_temp = millis();
  if ( abs(_now_temp - last_check) > waiting_time ) {
    retval = true;
  }
  return retval;
}

void readTemperatures() {
  if ( !needTempCheck(last_checked, CHECK_INTERVAL) ) {
    return;
  }
  last_checked = millis();
  Serial.println("START read temprature!!!!!");
  bool night=false;
  float alarm;
  TempSensorData sensor;
  for (int i=0; i< ROOMS; i++) {
    getHeatingSensor(i, sensor);
    Serial.println(sensor.name);
    last_temp[i]=sensorRead(sensor.addr);
    alarm=sensor.alarm;
    if ( night ) {
      alarm=sensor.night;
    }
    checkTemperature(last_temp[i], alarm, sensor.relay);
  }
  updateMainPump();
  Serial.println("STOP read temprature!!!!!");
}


void readExtraTemperatures() {
  if ( !needTempCheck(last_checked_extra, 2*CHECK_INTERVAL) ) {
    return;
  }
  last_checked_extra = millis();
  Serial.println("START read EXTRA temprature!!!!!");
  TempSensorData sensor;
  for (int i=ROOMS; i< ALL_SENSORS; i++) {
    getExtraSensor(i, sensor);
    Serial.println(sensor.name);
    last_temp[i]=sensorRead(sensor.addr);
  }
  Serial.println("STOP read EXTRA temprature!!!!!");
}
