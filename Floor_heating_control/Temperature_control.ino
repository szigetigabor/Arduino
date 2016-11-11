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

  for (int i=0; i< ROOMS; i++) {
    int eaddress = getSensorAddress(i);
    EEPROM.get(eaddress+ALARM_OFFSET, alarms[i]);//Serial.print(alarms[i]);Serial.println("");
    EEPROM.get(eaddress+NIGHT_OFFSET, nights[i]);//Serial.print(nights[i]);Serial.println("");
    EEPROM.get(eaddress+RELAY_OFFSET, relays[i]);//Serial.print(relays[i]);Serial.println("");
  }
  
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
  bool isnight=false;
  //float alarm;
  float night;
  //byte relay;
  bool old=true;
  old=false;
  //TempSensorData sensor;
  for (int i=0; i< ROOMS; i++) {
    byte addr;
    //alarm=0.0f;
    //night=0.0f;
    //relay=0;
    Serial.println("uj szenzor");
    Serial.println(i);
    Serial.println(alarms[i]);
    Serial.println(nights[i]);
    Serial.println(relays[i]);
    if ( old ) {
      TempSensorData sensor;
      //EEPROM.get(eaddress, sensor);
      getHeatingSensor(i, sensor);
      //Serial.println(sensor.name);
      Serial.println(sensor.alarm);
      Serial.println(sensor.night);
      Serial.println(sensor.relay);
      alarm=sensor.alarm;
    } else {
      int eaddress = getSensorAddress(i);
      Serial.print("    addr: ");
      EEPROM.get(eaddress, addr);Serial.print(addr, HEX);Serial.print(", ");
      EEPROM.get(eaddress+1, addr);Serial.print(addr, HEX);Serial.print(", ");
      EEPROM.get(eaddress+2, addr);Serial.print(addr, HEX);Serial.print(", ");
      EEPROM.get(eaddress+3, addr);Serial.print(addr, HEX);Serial.print(", ");
      EEPROM.get(eaddress+4, addr);Serial.print(addr, HEX);Serial.print(", ");
      EEPROM.get(eaddress+5, addr);Serial.print(addr, HEX);Serial.print(", ");
      EEPROM.get(eaddress+6, addr);Serial.print(addr, HEX);Serial.print(", ");
      EEPROM.get(eaddress+7, addr);Serial.print(addr, HEX);Serial.println("");
      
/*Serial.print("Starta: ");Serial.println(eaddress);
Serial.print("Alarma: ");Serial.println(eaddress+ALARM_OFFSET);
Serial.print("Deltaa: ");Serial.println(eaddress+DELTA_OFFSET);
Serial.print("Nighta: ");Serial.println(eaddress+NIGHT_OFFSET);
Serial.print("Relaya: ");Serial.println(eaddress+RELAY_OFFSET);
*/
/*Serial.print("    alarm: ");
EEPROM.get(eaddress+ALARM_OFFSET, addr);Serial.print(addr);Serial.println("");
byte alarma[4];
EEPROM.get(eaddress+28, alarma[0]);Serial.print(alarma[0]);Serial.println("");
EEPROM.get(eaddress+29, alarma[1]);//Serial.print(alarma[1]);Serial.println("");
EEPROM.get(eaddress+30, alarma[2]);//Serial.print(alarma[2]);Serial.println("");
EEPROM.get(eaddress+31, alarma[3]);//Serial.print(alarma[3]);Serial.println("");

//Serial.println((float)(alarma[0] << 24 | alarma[1] << 16 | alarma[2] << 8 | alarma[3]));
      //eaddress += ALARM_OFFSET;
     // float alarm;
      
//EEPROM.get(eaddress+ALARM_OFFSET, alarm);//Serial.print(alarm);Serial.println("");
      Serial.println(eaddress);
      //float tmp;
      // EEPROM.get(eaddress, sensor.addr);
      // EEPROM.get(eaddress+ALARM_OFFSET, alarm);
      //EEPROM.get(eaddress, alarm);
//      EEPROM.get(eaddress, tmp);
//      EEPROM.get(113, tmp);
//      Serial.println(tmp);
//      alarm = tmp;
//      delay(10);
//      Serial.print("Alarm:");
//       Serial.println(tmp);
 */      //Serial.println(alarm);
 /*   //eaddress += sizeof(float);
       EEPROM.get(eaddress+NIGHT_OFFSET, night);
    //EEPROM.get(eaddress, night);
    Serial.print("Night:");
       Serial.println(night);
//    eaddress += sizeof(float);
*/
      byte relay;
      EEPROM.get(eaddress+RELAY_OFFSET, relay);
      // EEPROM.get(eaddress, relay);
      Serial.print("    Relay:"); Serial.println(relay);




    /*  //  alarm=sensor.alarm;
    //alarm=getAlarm(i);
//    Serial.print("Alarm:");
    Serial.println(alarm);
    night=getNight(i);
    Serial.print("Night:");
    Serial.println(night);
    relay=getRelay(i);
    Serial.print("Relay:");
    Serial.println(relay);*/
    }
    

    //last_temp[i]=sensorRead(sensor.addr);
    //last_temp[i]=sensorRead(getAddr(i));

    if ( isnight ) {
      //alarm=sensor.night;
     // alarm=night;
    }
//    Serial.println(alarm);
//    Serial.println(night);
//    Serial.println(relay);
    //checkTemperature(last_temp[i], alarm, sensor.relay);
    //checkTemperature(last_temp[i], alarm, relay);
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
  return;
  TempSensorData sensor;
  for (int i=ROOMS; i< ALL_SENSORS; i++) {
    //getExtraSensor(i, sensor);
    //Serial.println(sensor.name);
    //last_temp[i]=sensorRead(sensor.addr);
    last_temp[i]=sensorRead(getAddr(i));
  }
  Serial.println("STOP read EXTRA temprature!!!!!");
}
