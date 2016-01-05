#include <EEPROM.h>
#include "Temp_config.h"

void initEEPROM() {

  int eeAddress = 0; //EEPROM address to start reading from
  bool reset = false;
  EEPROM.get( eeAddress, reset );
  if ( reset ) {
    EEPROM.put( eeAddress, false );
    Serial.println("reset set to false");
    for (int i=0; i<rooms; i++) {
    
      digitalWrite(ledPin, HIGH);   // sets the LED on
    
      // get() can be used with custom structures too.
      eeAddress = sizeof(reset)+i*sizeof(TempSensorData); //Move address to the next byte after float 'f'.
      TempSensorData SensorData; //Variable to store custom object read from EEPROM.
    
      if ( i == 0 ) {
        memcpy ( SensorData.addr, sensor0.addr, sizeof(sensor0.addr)+1 );   //SensorData.addr = sensor0.addr;
        SensorData.alarm = sensor0.alarm;
        SensorData.delta = sensor0.delta;
        strcpy (SensorData.name,sensor0.name);
      }
      if ( i == 1 ) {
        memcpy ( SensorData.addr, sensor1.addr, sizeof(sensor1.addr)+1 );   //SensorData.addr = sensor1.addr;
        SensorData.alarm = sensor1.alarm;
        SensorData.delta = sensor1.delta;
        strcpy (SensorData.name,sensor1.name);
      }
      if ( i == 2 ) {
        memcpy ( SensorData.addr, sensor2.addr, sizeof(sensor2.addr)+1 );   //SensorData.addr = sensor2.addr;
        SensorData.alarm = sensor2.alarm;
        SensorData.delta = sensor2.delta;
        strcpy (SensorData.name,sensor2.name);
      }
      if ( i == 3 ) {
        memcpy ( SensorData.addr, sensor3.addr, sizeof(sensor3.addr)+1 );   //SensorData.addr = sensor3.addr;
        SensorData.alarm = sensor3.alarm;
        SensorData.delta = sensor3.delta;
        strcpy (SensorData.name,sensor3.name);
      }
     

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


