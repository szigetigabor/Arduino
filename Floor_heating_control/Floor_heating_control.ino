//#include "EEPROM_control.h"
#include "Temp_config.h"


int ledPin = 13;                 // LED connected to digital pin 13
float last_temp[ALL_SENSORS];
bool relay_status[8];

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output

  //setReset();
  initEEPROM();
  dataCheck();

  initDisplay();
  Serial.println("End setup function");

}

void loop() {
  // put your main code here, to run repeatedly:
  readTemperatures();
  readExtraTemperatures();
  showGUI();
}



