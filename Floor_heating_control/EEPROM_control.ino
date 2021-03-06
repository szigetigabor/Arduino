

#include "EEPROM_control.h"
#include "Display_includes.h"


void initEEPROM() {

  int eeAddress = 0; //EEPROM address to start reading from
  bool reset = false;
  EEPROM.get( eeAddress, reset );
  if ( reset ) {
    EEPROM.put( eeAddress, false );
    Serial.println("reset set to false");

    setMode(MODE_ON);
    setSleep(SLEEP);
    setHeatingSensors();
    setExtraSensors();
  } else {
    idle_max = values[getSleep()];
  }
}

void setReset() {
  EEPROM.put( 0, true );
  Serial.println("reset set to true");
}

byte getMode() {
  byte value = EEPROM.read(1);
  return value;
}

void setMode(byte value) {
  EEPROM.put( 1, value );
  Serial.println("stored mode value");
}

byte getSleep() {
  byte value = EEPROM.read(2);
  return value;
}

void setSleep(byte value) {
  EEPROM.put( 2, value );
  Serial.println("stored sleep value");
}

void getHeatingSensor(int index, TempSensorData& sensorData) {
  if ( index>=0 && index<ROOMS ) {
    int addr = getSensorAddress(index);
    EEPROM.get(addr, sensorData);
  }
}

void setHeatingSensors() {
  int eeAddress;
  TempSensorData* currentSensor;
  for (int i=0; i<ROOMS; i++) {
  
    digitalWrite(BUSY_PIN, HIGH);   // sets the LED on
    
    // get() can be used with custom structures too.
    eeAddress = BASE_OFFSET+i*sizeof(TempSensorData); //Move address to the next byte after float 'f'.
    TempSensorData SensorData; //Variable to store custom object read from EEPROM.
    
    switch (i) {
      case 0:
        currentSensor = &sensor0;
        break;
      case 1:
        currentSensor = &sensor1;
        break;
      case 2:
        currentSensor = &sensor2;
        break;
      case 3:
        currentSensor = &sensor3;
        break;
      case 4:
        currentSensor = &sensor4;
        break;
    }

    memcpy ( SensorData.addr, currentSensor->addr, sizeof(currentSensor->addr)+1 );   //SensorData.addr = sensor3.addr;
    strcpy (SensorData.name,currentSensor->name);
    SensorData.alarm = currentSensor->alarm;
    SensorData.delta = currentSensor->delta;
    SensorData.night = currentSensor->night;
    SensorData.relay = currentSensor->relay;


    Serial.print(SensorData.addr[0], HEX);
    Serial.print(" ");
    Serial.print(SensorData.name);
    Serial.print(" write");
    EEPROM.put(eeAddress, SensorData);
      
    Serial.println("");
    delay(1000);                  // waits for a second
    digitalWrite(BUSY_PIN, LOW);    // sets the LED off
    delay(1000);
  }
}

void getExtraSensor(int index, TempSensorData& sensorData) {
  if ( index>=ROOMS && index<ALL_SENSORS ) {
    int addr = getSensorAddress(index);
    EEPROM.get(addr, sensorData);
  }
}

void setExtraSensors() {
  int eeAddress;
  int offset = BASE_OFFSET+ROOMS*sizeof(TempSensorData);
  TempSensor* currentSensor;
  for (int i=ROOMS; i<ALL_SENSORS; i++) {
      // get() can be used with custom structures too.
      eeAddress = offset+(i-ROOMS)*sizeof(TempSensor);
      TempSensor SensorData; //Variable to store custom object read from EEPROM.
    
      switch (i) {
        case 5:
          currentSensor = &sensor5;
          break;
        case 6:
          currentSensor = &sensor6;
          break;
        case 7:
          currentSensor = &sensor7;
          break;
        case 8:
          currentSensor = &sensor8;
          break;
        case 9:
          currentSensor = &sensor9;
          break;
        case 10:
          currentSensor = &sensor10;
          break;
        case 11:
          currentSensor = &sensor11;
          break;
        case 12:
          currentSensor = &sensor12;
          break;
      }

      memcpy ( SensorData.addr, currentSensor->addr, sizeof(currentSensor->addr)+1 );   //SensorData.addr = sensor3.addr;
      strcpy (SensorData.name,currentSensor->name);
     

      Serial.print(SensorData.addr[0]);
      Serial.print(" ");
      Serial.print(SensorData.name);
      Serial.print(" write");
      EEPROM.put(eeAddress, SensorData);
      
      Serial.println("");
    }  

}

void setAlarm(int index, float value) {
  if ( index < ROOMS ) {
  int eeAddress = BASE_OFFSET+index*sizeof(TempSensorData);
  eeAddress += sizeof(byte[8]);  //addr
  eeAddress += sizeof(char[20]);  //name
  
  EEPROM.put(eeAddress, value);
  }
}

byte sensoraddr[8];
byte* getAddr(int index) {
  if ( index < ROOMS ) {
    int addr = getSensorAddress(index);
    EEPROM.get(addr, sensoraddr);
  }
  return sensoraddr;
}

float getAlarm(int index) {
  float alarm;
  if ( index < ROOMS ) {
    int addr = getSensorAddress(index);
    addr += ALARM_OFFSET;
    EEPROM.get(addr, alarm);
  }
  return alarm;
}

float getNight(int index) {
  float night=0;
  if ( index < ROOMS ) {
    int addr = getSensorAddress(index);
    addr += NIGHT_OFFSET;
    EEPROM.get(addr, night);
  }
  return night;
}

byte getRelay(int index) {
  byte relay=0;
  if ( index < ROOMS ) {
    int addr = getSensorAddress(index);
    addr += RELAY_OFFSET;
    EEPROM.get(addr, relay);
  }
  return relay;
}

void dataCheck() {
  for(int i=0; i<ALL_SENSORS; i++) {
    int index = getSensorAddress(i);
    Serial.println(index);
    if ( i < ROOMS) {
      TempSensorData value;
      EEPROM.get(index, value);
      Serial.println(value.name);
      Serial.println(value.alarm);
      Serial.println(value.night);
      Serial.println(value.relay);
    } else {
      TempSensor value;
      EEPROM.get(index, value);
      Serial.println(value.name);
    }
  }
  EEPROMUsage();
}

void EEPROMUsage() {
  Serial.print("Usage: ");
  int used = getSensorAddress(ALL_SENSORS);
  Serial.print(used, DEC );
  Serial.print("/");
  Serial.print(EEPROM.length());
  Serial.print(" = ");
  float percent =(float)used*100/(float)EEPROM.length();
  Serial.print(percent, 2);
  Serial.println(" %");
}

byte getDayHour() {
  byte value = 0;
  EEPROM.get(SCHEDULING_OFFSET, value);
  return value;
}

void setDayHour(byte value) {
  int eeAddress = SCHEDULING_OFFSET;
  EEPROM.write( eeAddress, value );
  if ( debug >= DEBUG ) {
    Serial.print("stored scheduling day hour value ");Serial.println(value);
  }
}

byte getDayMinute() {
  byte value = 0;
  EEPROM.get(SCHEDULING_OFFSET+1, value);
  return value;
}

void setDayMinute(byte value) {
  int eeAddress = SCHEDULING_OFFSET+1;
  EEPROM.write( eeAddress, value );
  if ( debug >= DEBUG ) {
    Serial.print("stored scheduling day minute value ");Serial.println(value);
  }
}

byte getNightHour() {
  byte value = 0;
  EEPROM.get(SCHEDULING_OFFSET+2, value);
  return value;
}

void setNightHour(byte value) {
  int eeAddress = SCHEDULING_OFFSET+2;
  EEPROM.write( eeAddress, value );
  if ( debug >= DEBUG ) {
    Serial.print("stored scheduling night hour value ");Serial.println(value);
  }
}

byte getNightMinute() {
  byte value = 0;
  EEPROM.get(SCHEDULING_OFFSET+3, value);
  return value;
}

void setNightMinute(byte value) {
  int eeAddress = SCHEDULING_OFFSET+3;
  EEPROM.write( eeAddress, value );
  if ( debug >= DEBUG ) {
    Serial.print("stored scheduling night minute value ");Serial.println(value);
  }
}
