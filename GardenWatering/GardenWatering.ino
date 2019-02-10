#include "commonFunctions.h"
#include "WebServerLogic.h"
//#include "SchedulerLogic.h"
#include "MCPManagement.h"



//SchedulerLogic scheduler;
MCPManagement* buttons;



void setup(void){
  pinMode(led, OUTPUT);
  ledON();
  Serial.begin(115200);

  if (!bmp.begin()) {
    Serial.println("Could not find BMP180 or BMP085 sensor at 0x77");
    while (1) {}
  }

  WifiInit();

  webServerInit();
  ledOFF();

  scheduler.init();

  buttons = new MCPManagement();

}

void loop(void){
  server.handleClient();
  scheduler.execute();
  //printDB();
//  Serial.println(schedulerWebAdapter.getStartTime());


  buttons->buttonPushTriggerCheck();

}
