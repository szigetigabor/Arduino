#include <EEPROM.h>
#include "Temp_config.h"

void initEEPROM() {

  int eeAddress = 0; //EEPROM address to start reading from
  bool reset = false;
  EEPROM.get( eeAddress, reset );
  if ( reset ) {
    EEPROM.put( eeAddress, false );
    Serial.println("reset set to false");
    
    setHeatingSensors();
    setExtraSensors();
  }
}

void setReset() {
  EEPROM.put( 0, true );
  Serial.println("reset set to true");
}

void setHeatingSensors() {
  int eeAddress;
  TempSensorData* currentSensor;
  for (int i=0; i<rooms; i++) {
  
    digitalWrite(ledPin, HIGH);   // sets the LED on
    
    // get() can be used with custom structures too.
    eeAddress = sizeof(bool)+i*sizeof(TempSensorData); //Move address to the next byte after float 'f'.
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
  int offset = sizeof(bool)+rooms*sizeof(TempSensorData);
  TempSensor* currentSensor;
  for (int i=rooms; i<all_sensors; i++) {
      // get() can be used with custom structures too.
      eeAddress = offset+(i-rooms)*sizeof(TempSensor);
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
  if ( index < rooms ) {
  int eeAddress = sizeof(bool)+index*sizeof(TempSensorData);
  eeAddress += sizeof(byte[8]);  //addr
  eeAddress += sizeof(char[20]);  //name
  
  EEPROM.put(eeAddress, value);
  }
}

int getSensorAddress(int index) {
  int eeAddress = sizeof(bool);
  if ( index < rooms ) {
    eeAddress += index*sizeof(TempSensorData);
  } else if (index < all_sensors) {
    eeAddress += rooms*sizeof(TempSensorData);
    eeAddress += (index-rooms)*sizeof(TempSensor);
  }
  return eeAddress;
}

void dataCheck() {
  for(int i=0; i<all_sensors; i++) {
    int index = getSensorAddress(i);
    Serial.println(index);
    if ( i < rooms) {
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


