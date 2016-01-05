#include <EEPROM.h>
#include "Temp_config.h"

void initEEPROM() {

  int eeAddress = 0; //EEPROM address to start reading from
  bool reset = false;
  EEPROM.get( eeAddress, reset );
  if ( reset ) {
    EEPROM.put( eeAddress, false );
    Serial.println("reset set to false");
    TempSensorData* currentSensor;
    for (int i=0; i<rooms; i++) {
    
      digitalWrite(ledPin, HIGH);   // sets the LED on
    
      // get() can be used with custom structures too.
      eeAddress = sizeof(reset)+i*sizeof(TempSensorData); //Move address to the next byte after float 'f'.
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
}

void setReset() {
  EEPROM.put( 0, true );
  Serial.println("reset set to true");
}


