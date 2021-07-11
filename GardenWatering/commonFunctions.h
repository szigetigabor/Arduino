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

#define BATTERY_CHARGING_PORT 7
static bool BATTERY_CHARGING_OFF_ENABLE = false;
#define BATTERY_MIN_V  3.6
#define BATTERY_MAX_V  4.1
#define BATTERY_WARNING_LEVEL 25
#define BATTERY_ALERT_LEVEL   10

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

static float getBatteryPercentage() {
  float percentage = ((getBatteryVoltage() - BATTERY_MIN_V) / (BATTERY_MAX_V - BATTERY_MIN_V)) * 100;
  return percentage;
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
