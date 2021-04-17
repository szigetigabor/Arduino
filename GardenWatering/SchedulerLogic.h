//  SchedulerLogic.h - Arduino Scheduler Logic header that syncing Time form NTP for use with NtpClient, TimeAlarms libraries
#pragma once
#ifndef SCHEDULER_LOGIC_H
#define SCHEDULER_LOGIC_H

#include <Arduino.h>
#include <NtpClientLib.h>
#include <TimeAlarms.h>
#include "NTPConfig.h"
#include "ZoneConfig.h"
#include "PoolConfig.h"

class SchedulerLogic
{
public:
  SchedulerLogic(int NTPSyncPeriod = DEFAULT_SYNC_PERION);
  void init();
  void execute();

  void digitalClockDisplay();
  void printDigits(int digits);

  static bool isZonesEnabledFromRainSensor();

  bool isZoneActive();
  int getActiveZone();
  int getStartHour();
  void setStartHour(int hour);
  int getStartMinute();
  void setStartMinute(int minute);
  bool isAlarmsEnabled();
  void EnableZoneAlarms();
  void DisableZoneAlarms();

  bool isPoolActive();
  int getPoolStartHour();
  void setPoolStartHour(int hour);
  int getPoolStartMinute();
  void setPoolStartMinute(int minute);
  bool isPoolAlarmsEnabled();
  void EnablePoolAlarms();
  void DisablePoolAlarms();
  
protected:
  void AlarmsInit();
  void NTP_Init();
  void ZoneAlarmsInit();
  void ZoneAlarmsReInit();
  void ZoneAlarmsReset();

  void PoolAlarmsInit();
  void PoolAlarmsReInit();
  void PoolAlarmsReset();
  
private:
  int syncPeriod;   // NTP syncronization period in seconds

  int getZoneDuration(int i);
  bool zonesEnabled;
  int startHour;
  int startMinute;
  AlarmId ZoneONIds[NR_OF_ZONES];
  AlarmId ZoneOFFIds[NR_OF_ZONES];

  int getPoolTriggerFrequency();
  int getPoolDuration();
  bool poolEnabled;
  int poolStartHour;
  int poolStartMinute;
  AlarmId PoolIds[NR_OF_POOL_ALARMS];
};

#endif //SCHEDULER_LOGIC_H
