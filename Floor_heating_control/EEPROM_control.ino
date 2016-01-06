#include <EEPROM.h>
#include "Temp_config.h"

#define BACKLIGHT 20 
#define SLEEP 2 
/*
 * EEPROM map:
 *  1. reset
 *  2. blacklight
 *  3. sleep time
 *  4-x. room infos
 *  x-y. extra temperature sensors
 */
#define BASE_OFFSET 3
 
void initEEPROM() {

  int eeAddress = 0; //EEPROM address to start reading from
  bool reset = false;
  EEPROM.get( eeAddress, reset );
  if ( reset ) {
    EEPROM.put( eeAddress, false );
    Serial.println("reset set to false");

    setBacklight(BACKLIGHT);
    setSleep(SLEEP);
    setHeatingSensors();
    setExtraSensors();
  }
}

void setReset() {
  EEPROM.put( 0, true );
  Serial.println("reset set to true");
}

void setBacklight(int value) {
  EEPROM.put( 1, value );
  Serial.println("stored backlight value");
}

void setSleep(int value) {
  EEPROM.put( 2, value );
  Serial.println("stored sleep value");
}

void setHeatingSensors() {
  int eeAddress;
  TempSensorData* currentSensor;
  for (int i=0; i<ROOMS; i++) {
  
    digitalWrite(ledPin, HIGH);   // sets the LED on
    
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
    }

    memcpy ( SensorData.addr, currentSensor->addr, sizeof(currentSensor->addr)+1 );   //SensorData.addr = sensor3.addr;
    SensorData.alarm = currentSensor->alarm;
    SensorData.delta = currentSensor->delta;
    strcpy (SensorData.name,currentSensor->name);
   

    Serial.print(SensorData.addr[0]);
    Serial.print(" ");
    Serial.print(SensorData.name);
    Serial.print(" write");
    EEPROM.put(eeAddress, SensorData);
      
    Serial.println("");
    delay(1000);                  // waits for a second
    digitalWrite(ledPin, LOW);    // sets the LED off
    delay(1000);
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
        case 4:
          currentSensor = &sensor4;
          break;
        case 5:
          currentSensor = &sensor5;
          break;
        case 6:
          currentSensor = &sensor6;
          break;
        case 7:
          currentSensor = &sensor7;
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

int getSensorAddress(int index) {
  int eeAddress = BASE_OFFSET;
  if ( index < ROOMS ) {
    eeAddress += index*sizeof(TempSensorData);
  } else if (index < ALL_SENSORS) {
    eeAddress += ROOMS*sizeof(TempSensorData);
    eeAddress += (index-ROOMS)*sizeof(TempSensor);
  }
  return eeAddress;
}

void dataCheck() {
  for(int i=0; i<ALL_SENSORS; i++) {
    int index = getSensorAddress(i);
    Serial.println(index);
    if ( i < ROOMS) {
      TempSensorData value;
      EEPROM.get(index, value);
      Serial.println(value.name);
    } else {
      TempSensor value;
      EEPROM.get(index, value);
      Serial.println(value.name);
    }
  }
  
}



