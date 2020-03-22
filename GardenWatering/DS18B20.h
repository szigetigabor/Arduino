#pragma once
#ifndef DB18B20_H
#define DB18B20_H

#include <OneWire.h>
#include "TempConfig.h"

// OneWire DS18S20, DS18B20, DS1822 Temperature Example
//
// http://www.pjrc.com/teensy/td_libs_OneWire.html
//
// The DallasTemperature library can do all this work for you!
// https://github.com/milesburton/Arduino-Temperature-Control-Library


class DS18B20Temp
{
  public:
    DS18B20Temp(int pin, byte* _addr);

    void PrintSensorValue(bool debug);
    float getSensorValue();

    void PrintSensorAddress();
    
  private:
    OneWire ds;
    byte addr[8];
};


class DS18B20TempCollection
{
  public:
    DS18B20TempCollection();

    int getSensorCount();
    void PrintSensorValue(int index, bool debug);
    void PrintAllSensorValue(bool debug);
    float getSensorValue(int index);
    String getAllSensorValue();
    
  private:
    int sensorCount;
    DS18B20Temp* sensors[NR_OF_SENSORS];
};

#endif //DB18B20_H
