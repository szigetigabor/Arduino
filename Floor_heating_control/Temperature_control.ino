#include "Temp_config.h"
#include "Relay.h"
#include "Scheduling.h"
#include <OneWire.h>
#define ONE_WIRE_BUS 19                  // DS18B20 is on P19

OneWire ds(ONE_WIRE_BUS);

void printArray(byte* addr){
  for(int i = 0; i < 8; i++)
  {
    Serial.print(addr[i], HEX);
  }
  Serial.print(", ");
}

float sensorRead(byte* _addr)
{
  byte j;
  byte present = 0;
  byte data[12];

  if ( debug >= DEBUG ) {
    printArray(_addr);
  }
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

void checkTemperature(int index, boolean isnight) {
//    Serial.println(index);
//    Serial.println(last_temp[index]);
//    Serial.println(alarms[index]);
//    Serial.println(relays[index]);
    float req;
    memcpy ( &req, &alarms[index], sizeof(float) );
    byte rel;
    memcpy ( &rel, &relays[index], sizeof(byte) );
  //checkTemperature(last_temp[index], req, rel);
    if ( isnight ) {
      if ( debug >= DEBUG ) {
        Serial.println("night");
      }
      checkTemperature(last_temp[index], nights[index], relays[index]);
    } else {
      if ( debug >= DEBUG ) {
        Serial.println("day");
      }
      checkTemperature(last_temp[index], alarms[index], relays[index]);
    }
}

void checkTemperature(float current, float required, byte relay) {
  bool output = OFF;
  if (  ( current < required && current_mode == MODE_SCHEDULING )
      || current_mode == MODE_ON) {
    output = ON;
  }
  if ( debug >= DEBUG ) {
    Serial.print(" current: "); Serial.print(current);
    Serial.print(" required: ");Serial.print(required);
    Serial.print(" relay: ");   Serial.println(relay);
  }
  if ( relay_status[relay % 8] != output ) {
    setRelay(relay, output);
    relay_status[relay % 8] = output;
  }
}

void updateMainPump() {
  bool value = false;
  if ( current_mode == MODE_ON ) {
    value = true;
  } else if ( current_mode == MODE_SCHEDULING ) {
    for (int i=0; i< ROOMS; i++) {
      value = value | !relay_status[i];
    }
    value = value & boilerIsRunning;
  }
  // safety protection
  if ( debug >= DEBUG ) {
    Serial.print("main pump:");Serial.println(!relay_status[MAIN_PUMP % 8]);
    Serial.print("new value:");Serial.println(!value);
  }
  if (relay_status[MAIN_PUMP % 8] && value) {   //inverse logic
    // wait a few seconds until an "Electrothermal actuators" to open a pipes
    Serial.println("Waiting for a pipe opening...");
    delay(45000);  //45 seconds
  }
  setRelay(MAIN_PUMP, !value);
  relay_status[MAIN_PUMP % 8] = !value;   //only for mainGUI
}

void initTemp() {
  last_checked = millis();
  last_checked_extra  = millis();

  for (int i=0; i< ROOMS; i++) {
    int eaddress = getSensorAddress(i);
    EEPROM.get(eaddress+ALARM_OFFSET, alarms[i]);//Serial.print(alarms[i]);Serial.println("");
    EEPROM.get(eaddress+NIGHT_OFFSET, nights[i]);//Serial.print(nights[i]);Serial.println("");
    EEPROM.get(eaddress+RELAY_OFFSET, relays[i]);//Serial.print(relays[i]);Serial.println("");
    pinMode(relays[i], OUTPUT);
  }
  current_mode = getMode();
  pinMode(MAIN_PUMP, OUTPUT);
  // initialy turn off the main pump
  relay_status[MAIN_PUMP % 8]=1;
  setRelay(MAIN_PUMP, true);
  pinMode(BUSY_PIN, OUTPUT);
  digitalWrite(BUSY_PIN, OFF);
  initScheduling();
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
  boilerIsRunning = boilerRunningCheck();
  if ( debug >= DEBUG ) {
    Serial.println("START read temprature!!!!!");
  }
  digitalWrite(BUSY_PIN, ON);
  bool isnight=false;
  float alarm;
  isnight=isNight();
  for (int i=0; i< ROOMS; i++) {
    if ( debug >= DEBUG ) {
      Serial.print(i);
      Serial.print(". sensor: ");
    }
  
    TempSensorData sensor;
    getHeatingSensor(i, sensor);
    last_temp[i]=sensorRead(sensor.addr);
    alarm=sensor.alarm;
    if ( isnight ) {
      alarm=sensor.night;
    }
    //checkTemperature((float)last_temp[i], (float)alarms[i], (byte)relays[i]);
    //checkTemperature(last_temp[i], alarms[i], relays[i]);
    //checkTemperature(i, isnight);
    checkTemperature(last_temp[i], alarm, sensor.relay);
  }
  updateMainPump();
  digitalWrite(BUSY_PIN, OFF);
  if ( debug >= DEBUG ) {
    Serial.println("STOP read temprature!!!!!");
  }
}


void readExtraTemperatures() {
  if ( !needTempCheck(last_checked_extra, 2*CHECK_INTERVAL) ) {
    return;
  }
  last_checked_extra = millis();
  if ( debug >= DEBUG ) {
    Serial.println("START read EXTRA temprature!!!!!");
  }
  digitalWrite(BUSY_PIN, ON);
  return;
  TempSensorData sensor;
  for (int i=ROOMS; i< ALL_SENSORS; i++) {
    //getExtraSensor(i, sensor);
    //Serial.println(sensor.name);
    //last_temp[i]=sensorRead(sensor.addr);
    last_temp[i]=sensorRead(getAddr(i));
  }
  digitalWrite(BUSY_PIN, OFF);
  if ( debug >= DEBUG ) {
    Serial.println("STOP read EXTRA temprature!!!!!");
  }
}

