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


#endif //COMMON_FUNCTIONS_H
