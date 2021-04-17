#pragma once
#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H

/*
 * LED methods
 */
const int led = BUILTIN_LED; //13;

static void ledOFF(){
  digitalWrite(led, 1);
  Serial.println("Led turn Off.");
}

static void ledON(){
  digitalWrite(led, 0);
  Serial.println("Led turn On.");
}


/*
 * Battery management functions
 */
#define BATTERY_PORT A0

static void initBatteryChecking() {
  pinMode(BATTERY_PORT, INPUT);
}

static float getBatteryVoltage() {
  unsigned int raw=0;
  float volt=0.0;
  raw = analogRead(BATTERY_PORT);
  volt=raw/1023.0;
  volt=volt*4.2;
  return volt;
}


/*
 * Rain Sensor functions
 */
const int RainSensor = D6;

static int readRainSensor() {
  return digitalRead(RainSensor);
}

static bool enableZonesFromSensor() {
    if (readRainSensor()==HIGH)
    {
      Serial.println("RainSensor pressed!");
      return false;
    }
    else
    {
       Serial.println("RainSensor released!");
       return true;
    }
}

#endif //COMMON_FUNCTIONS_H
