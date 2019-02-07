//  SchedulerLogic.h - Arduino Scheduler Logic header that syncing Time form NTP for use with NtpClient, TimeAlarms libraries
#pragma once
#ifndef SCHEDULER_LOGIC_H
#define SCHEDULER_LOGIC_H

#include <Arduino.h>
#include <NtpClientLib.h>
#include <TimeAlarms.h>
#include "NTPConfig.h"
#include "ZoneConfig.h"

class SchedulerLogic
{
public:
  SchedulerLogic(int NTPSyncPeriod = DEFAULT_SYNC_PERION);
  void init();
  void execute();

  void digitalClockDisplay();
  void printDigits(int digits);

  int getStartHour();
  void setStartHour(int hour);
  int getStartMinute();
  void setStartMinute(int minute);
  bool isAlarmsEnabled();
  void EnableZoneAlarms();
  void DisableZoneAlarms();
  
protected:
  void AlarmsInit();
  void NTP_Init();
  void ZoneAlarmsInit();
  void ZoneAlarmsReInit();
  void ZoneAlarmsReset();
  
private:
  int getZoneDuration(int i);
  int syncPeriod;   // NTP syncronization period in seconds

  bool enabled;
  int startHour;
  int startMinute;

  AlarmId ZoneONIds[NR_OF_ZONES];
  AlarmId ZoneOFFIds[NR_OF_ZONES];
};

#endif //SCHEDULER_LOGIC_H
