#include "commonFunctions.h"
#include "WebServerLogic.h"
//#include "SchedulerLogic.h"

#include "Adafruit_MCP23017.h"


//SchedulerLogic scheduler;
Adafruit_MCP23017 mcp;

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


  mcp.begin(0); // use default address 0
  
  mcp.pinMode(8, OUTPUT);
  mcp.pullUp(8, HIGH);  // turn on a 100K pullup internally
  mcp.pinMode(10, OUTPUT);
  mcp.pullUp(10, HIGH);  // turn on a 100K pullup internally
}

void loop(void){
  server.handleClient();
  scheduler.execute();
  //printDB();
//  Serial.println(schedulerWebAdapter.getStartTime());


  delay(1000);

  ledON();
  mcp.digitalWrite(10, HIGH);
  mcp.digitalWrite(8, HIGH);

  delay(1000);

  ledOFF();
  mcp.digitalWrite(10, LOW);
  mcp.digitalWrite(8, LOW);
}
