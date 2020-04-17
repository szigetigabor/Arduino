#pragma once
#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H

const int led = BUILTIN_LED; //13;

void ledOFF(){
  digitalWrite(led, 1);
  Serial.println("Led turn Off.");
}

void ledON(){
  digitalWrite(led, 0);
  Serial.println("Led turn On.");
}

#define BATTERY_PORT A0

void initBatteryChecking() {
  pinMode(BATTERY_PORT, INPUT);
}

float getBatteryVoltage() {
  unsigned int raw=0;
  float volt=0.0;
  raw = analogRead(BATTERY_PORT);
  volt=raw/1023.0;
  volt=volt*4.2;
  return volt;
}

#endif //COMMON_FUNCTIONS_H
