#include "commonFunctions.h"
#include "WebServerLogic.h"
#include "Cayenne.h"

void setup(void) {
  pinMode(RainSensor, INPUT_PULLUP);
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
  initBatteryChecking();
  CayenneInit();
}

void loop(void) {
  WifiConnectionCheck();
  server.handleClient();
  scheduler.execute();

  getButtonPtr()->buttonPushTriggerCheck();
  getMomentaryButtonPtr()->buttonPushTriggerCheck();

  CayenneRun();
}
