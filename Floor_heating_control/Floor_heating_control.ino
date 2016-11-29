//#include "EEPROM_control.h"
#include "Temp_config.h"
#include "RTC_control.h"
#include "sct013.h"


int ledPin = 13;                 // LED connected to digital pin 13
float last_temp[ALL_SENSORS];
bool relay_status[8];            //current status of relay pins
bool boilerIsRunning;


void setup() {
  // put your setup code here, to run once:

  last_temp[0]=20.0;    //only for debug
  last_temp[1]=21.2;
  last_temp[2]=24.2;
  last_temp[3]=20.7;
  last_temp[4]=23.7;
  last_temp[5]=22.2;
  last_temp[6]=19.2;
  last_temp[7]=18.3;
  last_temp[8]=-5.3;
  last_temp[9]=17.9;
  last_temp[10]=5.8;
  last_temp[11]=9.8;
  last_temp[12]=5.8;
  
  Serial.begin(38400);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output

  boilerIsRunning = true;
  //setRTCatCompilation();      // set RTC at compilation time
  //setReset();                 // only first time

  readRTC();

  initEEPROM();               // write default values to eeprom, if setReset is activated (it needs only first time)
  initSCT();
  initTemp();
  //dataCheck();

  initDisplay();
  Serial.println("End setup function");

}

void loop() {
  // put your main code here, to run repeatedly:
  readRTC();
  boilerIsRunning = boilerRunningCheck();
  readTemperatures();
  readExtraTemperatures();
  showGUI();
}




