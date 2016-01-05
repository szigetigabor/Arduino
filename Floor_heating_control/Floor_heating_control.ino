#include <EEPROM.h>

struct TempSensorData {
  byte addr[8];
  float alarm;
  float delta;
  char name[20];
};

TempSensorData sensor0 = {
      {0x28, 0xFF, 0x7C, 0x89, 0x3D, 0x04, 0x00, 0xBB},
      0.00f,
      0.1f,
      "konyha"
};

TempSensorData sensor1 = {
      {0x28, 0xC0, 0x10, 0x8F, 0x04, 0x00, 0x00, 0x20},
      0.00f,
      0.1f,
      "etkezo"
};
TempSensorData sensor2 = {
      {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
      0.00f,
      0.1f,
      "furdo"
};
TempSensorData sensor3 = {
      {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
      0.00f,
      0.1f,
      "eloszoba"
};



int ledPin = 13;                 // LED connected to digital pin 13
int rooms  = 4;

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output

  //setReset();
  initEEPROM();
    
  Serial.print("End setup function");

}

void loop() {
  // put your main code here, to run repeatedly:

}

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


