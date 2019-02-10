#include "SchedulerLogic.h"
#include <time.h>       /* time_t, struct tm, time, localtime */


//**************************************************************
//* External Alarm Methods
AlarmId id;

// functions to be called when an alarm triggers:
void MorningAlarm() {
  Serial.println("Alarm: - turn lights off");
}

void EveningAlarm() {
  Serial.println("Alarm: - turn lights on");
}

void setDigitalOutput(int zoneID, int value) {
  int index = zoneID-1;
  if ( 0 <= index && index < DEFAULT_ACTIVE_TIME_ZONE ) {
    int pin = zonePortMap[index];
    digitalWrite(pin, value);  // 0: ON; 1: OFF
  } 
}

void ZoneON(int i) {
  Serial.println("Alarm: - Zone" + String(i) + " turn on");
  setDigitalOutput(i, LOW);  // turn ON
}

void ZoneOFF(int i) {
  Serial.println("Alarm: - Zone" + String(i) + " turn off");
  setDigitalOutput(i, HIGH);  // turn OFF
}

// Zone 1
void Zone1ON() {
  ZoneON(1);
}

void Zone1OFF() {
  ZoneOFF(1);
}

// Zone 2
void Zone2ON() {
  ZoneON(2);
}

void Zone2OFF() {
  ZoneOFF(2);
}

// Zone 3
void Zone3ON() {
  ZoneON(3);
}

void Zone3OFF() {
  ZoneOFF(3);
}

typedef void (*FuncPtr)(void);  //typedef 'return type' (*FuncPtr)('arguments')
FuncPtr ZoneONFunctions[] = {&Zone1ON, &Zone2ON, &Zone3ON};//, &Zone4ON, &Zone5ON};
FuncPtr ZoneOFFFunctions[] = {&Zone1OFF, &Zone2OFF, &Zone3OFF};//, &Zone4OFF, &Zone5OFF};


void WeeklyAlarm() {
  Serial.println("Alarm: - its Monday Morning");
}

void ExplicitAlarm() {
  Serial.println("Alarm: - this triggers only at the given date and time");
}

void Repeats() {
  Serial.println("15 second timer");
}

void HourlyEvent() {
  digitalWrite(BUILTIN_LED, 0); //ledON();
  Serial.println("Hourly event");
  delay(500);
  digitalWrite(BUILTIN_LED, 1); //ledOFF();
}

void Repeats2() {
  Serial.println("2 second timer");
}

void OnceOnly() {
  Serial.println("This timer only triggers once, stop the 2 second timer");
  // use Alarm.free() to disable a timer and recycle its memory.
  Alarm.free(id);
  // optional, but safest to "forget" the ID after memory recycled
  id = dtINVALID_ALARM_ID;
  // you can also use Alarm.disable() to turn the timer off, but keep
  // it in memory, to turn back on later with Alarm.enable().
}


//**************************************************************
//* SchedulerLogic Class Constructor

SchedulerLogic::SchedulerLogic(int NTPSyncPeriod)
{
  syncPeriod  = NTPSyncPeriod;
  startHour   = DEFAULT_START_HOUR;
  startMinute = DEFAULT_START_MINUTE;
  enabled     = true;
  Serial.begin(115200);
  while (!Serial) ; // wait for Arduino Serial Monitor

  setTime(8, 29, 0, 1, 1, 11); // set time to Saturday 8:29:00am Jan 1 2011
}

//**************************************************************
//* Public Methods

void SchedulerLogic::init() {
  Serial.println("Scheduler logic initialization...");
  AlarmsInit();
  NTP_Init();
  Serial.println("Scheduler logic initialized!");
}

void SchedulerLogic::execute() {
  digitalClockDisplay();
  Alarm.delay(1000); // wait one second between clock display
}

void SchedulerLogic::digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println();
}

void SchedulerLogic::printDigits(int digits) {
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


int SchedulerLogic::getStartHour() {
  return startHour;
}

void SchedulerLogic::setStartHour(int hour) {
  if ( hour >= 0 && hour <= 23) {
    startHour = hour;
    ZoneAlarmsReInit();
  }
}

int SchedulerLogic::getStartMinute() {
  return startMinute;
}

void SchedulerLogic::setStartMinute(int minute) {
  if ( minute >= 0 && minute <= 59) {
    startMinute = minute;
    ZoneAlarmsReInit();
  }
}

bool SchedulerLogic::isAlarmsEnabled() {
  return enabled;
}

void SchedulerLogic::EnableZoneAlarms() {
  Serial.println("Zone alarms are enabled.");
  for (int i = 0; i < NR_OF_ZONES; i++) {
    Alarm.enable(ZoneONIds[i]);
    Alarm.enable(ZoneOFFIds[i]);
  }
  enabled = true;
}

void SchedulerLogic::DisableZoneAlarms() {
  Serial.println("Zone alarms are disabled.");
  for (int i = 0; i < NR_OF_ZONES; i++) {
    Alarm.disable(ZoneONIds[i]);
    Alarm.disable(ZoneOFFIds[i]);
  }
  enabled = false;
}

  
//**************************************************************
//* Protected Methods

void SchedulerLogic::AlarmsInit()
{
  Serial.println("Alarms initialization...");

  // create the alarms, to trigger at specific times
  ZoneAlarmsInit();
  
  Alarm.alarmRepeat(8,30,0, MorningAlarm);  // 8:30am every day
  Alarm.alarmRepeat(17,45,0,EveningAlarm);  // 5:45pm every day
  // Alarm.alarmRepeat(dowSaturday,8,30,30,WeeklyAlarm);  // 8:30:30 every Saturday

  // create timers, to trigger relative to when they're created
  Alarm.timerRepeat(15, Repeats);           // timer for every 15 seconds
  Alarm.timerRepeat(3600, HourlyEvent);           // timer for every hours
  id = Alarm.timerRepeat(2, Repeats2);      // timer for every 2 seconds
  Alarm.timerOnce(10, OnceOnly);            // called once after 10 seconds
}

void SchedulerLogic::ZoneAlarmsReInit()
{
  Serial.println("ZoneAlarms Reinitialization...");
  ZoneAlarmsReset();
  ZoneAlarmsInit();
}

void SchedulerLogic::ZoneAlarmsInit()
{
  Serial.println("ZoneAlarms Initialization...");
  time_t alarm = startHour * 3600 + startMinute * 60;

  int hour;
  int minute;
  for (int i = 0; i < NR_OF_ZONES; i++) {

    hour = (alarm / 3600) % 24;
    minute = (alarm / 60) % 60;

    // create the alarms, to trigger at specific times
    AlarmId id = Alarm.alarmRepeat(hour, minute, 1, ZoneONFunctions[i]);
    ZoneONIds[i] = id;

    int duration = getZoneDuration(i);
    alarm += duration * 60;
    hour = (alarm / 3600) % 24;
    minute = (alarm / 60) % 60;
    //id = Alarm.alarmRepeat(hour, minute, 0, Zone1OFF);
    id = Alarm.alarmRepeat(hour, minute, 0, ZoneOFFFunctions[i]);
    
    ZoneOFFIds[i] = id;
  }
}

void SchedulerLogic::ZoneAlarmsReset()
{
  Serial.println("ZoneAlarms Reseting...");
  for (int i = 0; i < NR_OF_ZONES; i++) {
    Alarm.free(ZoneONIds[i]);
    Alarm.free(ZoneOFFIds[i]);
    // optional, but safest to "forget" the ID after memory recycled
    ZoneONIds[i] = dtINVALID_ALARM_ID;
    ZoneOFFIds[i] = dtINVALID_ALARM_ID;
  }
}

void SchedulerLogic::NTP_Init()
{
  Serial.println("NTP Client initialization...");
  NTP.begin (NTP_SERVER_NAME, TIMEZONE, false, MINUTES_TIMEZONE);
  NTP.setInterval (syncPeriod);   // sync period in seconds
}


//**************************************************************
//* Private Methods

int SchedulerLogic::getZoneDuration(int i) {
  int duration = DEFAULT_ACTIVE_TIME_ZONE;
  switch (i) {
    case 0:
      #ifdef ACTIVE_TIME_ZONE1
        duration = ACTIVE_TIME_ZONE1;
      #endif
      break;
    case 1:
      #ifdef ACTIVE_TIME_ZONE2
        duration = ACTIVE_TIME_ZONE2;
      #endif
      break;
    case 2:
      #ifdef ACTIVE_TIME_ZONE3
        duration = ACTIVE_TIME_ZONE3;
      #endif
      break;
    case 3:
      #ifdef ACTIVE_TIME_ZONE4
        duration = ACTIVE_TIME_ZONE4;
      #endif
      break;
    case 4:
      #ifdef ACTIVE_TIME_ZONE5
        duration = ACTIVE_TIME_ZONE5;
      #endif
      break;
    default:
      duration = DEFAULT_ACTIVE_TIME_ZONE;
      break;
  }
  Serial.print("duration: ");
  Serial.print(duration);
  Serial.println(" seconds");
  return duration;
}