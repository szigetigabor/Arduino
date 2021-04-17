#include "SchedulerLogic.h"
#include <time.h>       /* time_t, struct tm, time, localtime */
#include "MCPManagement.h"
#include "MCPConfig.h"
#include "commonFunctions.h"


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

//*********************************
//* MCP management
//* 
//* It manages all Zones and Pool
//*
//extern MCPManagement buttons;
//extern MCPMomentaryManagement momentary;
//MCPManagement* MCP = &momentary;
MCPMomentaryManagement* MCP = getMomentaryButtonPtr();
void setDigitalOutput(int zoneID, int value) {
  int index = zoneID-1;
  if ( (0 <= index && index < NR_OF_ZONES)   // zone update
        || zoneID == POOL_PORT) {   // Pool trigger
    MCP->setOutput(index, value);
  } 
}

void setMomentaryDigitalOutput(int zoneID) {
  setDigitalOutput(zoneID, LOW);  // turn ON
  delay(1000);
  setDigitalOutput(zoneID, HIGH);  // turn OFF  
}

//*********************************
//* Zone functions
//*  MCP port 1-5
//*
bool needToUseZones;
void ZoneON(int i) {
  if(needToUseZones) {
    Serial.println("Alarm: - Zone" + String(i) + " turn on");
    if (bZoneButtonMomentary) {
      if ( ActiveZone != i) {
        setMomentaryDigitalOutput(i);
      }
    }
    else {
      setDigitalOutput(i, LOW);  // turn ON
    }
    ActiveZone = i;
    delay(500);
  } else {
    Serial.println("Alarm: - Zone" + String(i) + " turn on is disabled by RainSensor");
  }
}

void ZoneOFF(int i) {
  if(needToUseZones) {
    Serial.println("Alarm: - Zone" + String(i) + " turn off");
    if (bZoneButtonMomentary) {
      if ( ActiveZone == i) {
        setMomentaryDigitalOutput(i);
      }
    }
    else {
      setDigitalOutput(i, HIGH);  // turn OFF
    }
    ActiveZone = -1;
    delay(500);
  } else {
    Serial.println("Alarm: - Zone" + String(i) + " turn off is disabled by RainSensor");
  }
}

// Zone 1
void Zone1ON() {
  needToUseZones = SchedulerLogic::isZonesEnabledFromRainSensor();
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

// Zone 4
void Zone4ON() {
  ZoneON(4);
}

void Zone4OFF() {
  ZoneOFF(4);
}

// Zone 5
void Zone5ON() {
  ZoneON(5);
}

void Zone5OFF() {
  ZoneOFF(5);
}

typedef void (*FuncPtr)(void);  //typedef 'return type' (*FuncPtr)('arguments')
FuncPtr ZoneONFunctions[] = {&Zone1ON, &Zone2ON, &Zone3ON, &Zone4ON, &Zone5ON};
FuncPtr ZoneOFFFunctions[] = {&Zone1OFF, &Zone2OFF, &Zone3OFF, &Zone4OFF, &Zone5OFF};

//*********************************
//* Pool alarm functions
//*
void PoolPumpTrigger() {
  Serial.println("Pool pump trigger");
  if (bPoolButtonMomentary) {
    setMomentaryDigitalOutput(POOL_PORT);
  }
  else {
    setDigitalOutput(POOL_PORT, bPoolActive);
  }
  bPoolActive = !bPoolActive;
}

//*********************************
//* Battery functions
//*
#define BATTERY_CHARGING_PORT 7
#define BATTERY_MIN_V  3.5
#define BATTERY_MAX_V  3.95

float getBatteryVoltage();

void BatteryChargeCheck() {
  Serial.println("Battery Charging Check...");
  float currentVoltage = getBatteryVoltage();
  Serial.print("  Current voltage: ");
  Serial.print(currentVoltage);
  Serial.println(" V.");
  if ( currentVoltage > BATTERY_MAX_V ) {
    // Battery Full, charging OFF
    getButtonPtr()->setOutput(BATTERY_CHARGING_PORT, true);
    Serial.println("   Charging OFF.");
  }
  if ( currentVoltage < BATTERY_MIN_V ) {
    // Battery Empty, charging ON

    getButtonPtr()->setOutput(BATTERY_CHARGING_PORT, false);
    Serial.println("   Charging ON.");
  }
}

// example alarms
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
  zonesEnabled= true;
  ActiveZone  = -1;
  poolStartHour   = DEFAULT_POOL_START_HOUR;
  poolStartMinute = DEFAULT_POOL_START_MINUTE;
  poolEnabled = true;
  bPoolActive = false;
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

bool SchedulerLogic::isZonesEnabledFromRainSensor() {
  return enableZonesFromSensor();  
}

bool SchedulerLogic::isZoneActive() {
  return getActiveZone() != -1;
}

int SchedulerLogic::getActiveZone() {
  return ActiveZone;
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
  return zonesEnabled;
}

void SchedulerLogic::EnableZoneAlarms() {
  Serial.println("Zone alarms are enabled.");
  for (int i = 0; i < NR_OF_ZONES; i++) {
    Alarm.enable(ZoneONIds[i]);
    Alarm.enable(ZoneOFFIds[i]);
  }
  zonesEnabled = true;
}

void SchedulerLogic::DisableZoneAlarms() {
  Serial.println("Zone alarms are disabled.");
  for (int i = 0; i < NR_OF_ZONES; i++) {
      ZoneOFF(i);
    Alarm.disable(ZoneONIds[i]);
    Alarm.disable(ZoneOFFIds[i]);
  }
  zonesEnabled = false;
}

bool SchedulerLogic::isPoolActive() {
  return bPoolActive;
}

int SchedulerLogic::getPoolStartHour() {
  return poolStartHour;
}

void SchedulerLogic::setPoolStartHour(int hour) {
  if ( hour >= 0 && hour <= 23) {
    poolStartHour = hour;
    PoolAlarmsReInit();
  }
}

int SchedulerLogic::getPoolStartMinute() {
  return poolStartMinute;
}

void SchedulerLogic::setPoolStartMinute(int minute) {
  if ( minute >= 0 && minute <= 59) {
    poolStartMinute = minute;
    PoolAlarmsReInit();
  }
}


bool SchedulerLogic::isPoolAlarmsEnabled() {
  return poolEnabled;
}

void SchedulerLogic::EnablePoolAlarms() {
  Serial.println("Pool alarms are enabled.");
  for (int i = 0; i < NR_OF_POOL_ALARMS; i++) {
    Alarm.enable(PoolIds[i]);
  }
  poolEnabled = true;
}

void SchedulerLogic::DisablePoolAlarms() {
  Serial.println("Pool alarms are disabled.");
  for (int i = 0; i < NR_OF_POOL_ALARMS; i++) {
    Alarm.disable(PoolIds[i]);
  }
  poolEnabled = false;
}

  
//**************************************************************
//* Protected Methods

void SchedulerLogic::AlarmsInit()
{
  Serial.println("Alarms initialization...");


  // Battery chargeing check
  Alarm.timerRepeat(300, BatteryChargeCheck);           // timer for every 5 minutes

  // create the alarms, to trigger at specific times
  ZoneAlarmsInit();

  // Pool circular pump
  PoolAlarmsInit();
 // Alarm.timerRepeat(15, Repeats);           // timer for every 15 seconds

/*
  // extra alarm examples
  Alarm.alarmRepeat(8,30,0, MorningAlarm);  // 8:30am every day
  Alarm.alarmRepeat(17,45,0,EveningAlarm);  // 5:45pm every day
  // Alarm.alarmRepeat(dowSaturday,8,30,30,WeeklyAlarm);  // 8:30:30 every Saturday

  // create timers, to trigger relative to when they're created
  Alarm.timerRepeat(15, Repeats);           // timer for every 15 seconds
  Alarm.timerRepeat(3600, HourlyEvent);           // timer for every hours
  id = Alarm.timerRepeat(2, Repeats2);      // timer for every 2 seconds
  Alarm.timerOnce(10, OnceOnly);            // called once after 10 seconds
  */
}

// ZONE methods
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
    AlarmId id = Alarm.alarmRepeat(hour, minute, DEFAULT_START_SECOND, ZoneONFunctions[i]);
    ZoneONIds[i] = id;

    int duration = getZoneDuration(i);
    alarm += duration * 60;
    hour = (alarm / 3600) % 24;
    minute = (alarm / 60) % 60;
    //id = Alarm.alarmRepeat(hour, minute, 0, Zone1OFF);
    id = Alarm.alarmRepeat(hour, minute, DEFAULT_START_SECOND + END_SHIFTING_TIME, ZoneOFFFunctions[i]);
    
    ZoneOFFIds[i] = id;
  }
}

void SchedulerLogic::ZoneAlarmsReset()
{
  Serial.println("ZoneAlarms Reseting...");
  for (int i = 0; i < NR_OF_ZONES; i++) {
    ZoneOFF(i);
    Alarm.free(ZoneONIds[i]);
    Alarm.free(ZoneOFFIds[i]);
    // optional, but safest to "forget" the ID after memory recycled
    ZoneONIds[i] = dtINVALID_ALARM_ID;
    ZoneOFFIds[i] = dtINVALID_ALARM_ID;
  }
  ActiveZone = -1;
}
// end of ZONE methods

// POOL methods
void SchedulerLogic::PoolAlarmsReInit()
{
  Serial.println("PoolAlarms Reinitialization...");
  PoolAlarmsReset();
  PoolAlarmsInit();
}

void SchedulerLogic::PoolAlarmsInit()
{
  Serial.println("PoolAlarms Initialization...");
  if (FIX_SCHEDULING) {
      PoolIds[0] = Alarm.alarmRepeat(6, 0, 0, PoolPumpTrigger);
      PoolIds[1] = Alarm.alarmRepeat(9, 0, 0, PoolPumpTrigger);

      PoolIds[2] = Alarm.alarmRepeat(12,0, 0, PoolPumpTrigger);
      PoolIds[3] = Alarm.alarmRepeat(12,30, 0, PoolPumpTrigger);

      PoolIds[4] = Alarm.alarmRepeat(16, 0, 0, PoolPumpTrigger);
      PoolIds[5] = Alarm.alarmRepeat(18, 30, 0, PoolPumpTrigger);
  }
  else {
      time_t alarm = poolStartHour * 3600 + poolStartMinute * 60;
    
      //PoolIds[0] = Alarm.alarmRepeat(18,00,0, PoolPumpTrigger);  // 18:00pm every day
      //PoolIds[1] = Alarm.alarmRepeat(0,01,0,PoolPumpTrigger);  // 0:01am every day
      int hour;
      int minute;
    
      int duration = getPoolDuration();
      int frequency = getPoolTriggerFrequency();
      for (int i = 0; i < NR_OF_POOL_ALARMS; i=i+2) {
    
        hour = (alarm / 3600) % 24;
        minute = (alarm / 60) % 60;
        Serial.print(hour);
        Serial.print(":");
        Serial.println(minute);
        // create the alarms, to trigger at specific times
        AlarmId id = Alarm.alarmRepeat(hour, minute, 0, PoolPumpTrigger);
        PoolIds[i] = id;
    
        time_t Offalarm(alarm);
        Offalarm += duration * 60;
        hour = (Offalarm / 3600) % 24;
        minute = (Offalarm / 60) % 60;
        Serial.print(hour);
        Serial.print(":");
        Serial.println(minute);
        id = Alarm.alarmRepeat(hour, minute, 0, PoolPumpTrigger);
        PoolIds[i+1] = id;
    
        alarm += frequency * 3600;
      }
  }
}

void SchedulerLogic::PoolAlarmsReset()
{
  Serial.println("PoolAlarms Reseting...");
  for (int i = 0; i < NR_OF_POOL_ALARMS; i++) {
    Alarm.free(PoolIds[i]);
    // optional, but safest to "forget" the ID after memory recycled
    PoolIds[i] = dtINVALID_ALARM_ID;
  }
  bPoolActive = false;
}
// end of POOL methods

void SchedulerLogic::NTP_Init()
{
  Serial.println("NTP Client initialization...");
  NTP.begin (NTP_SERVER_NAME, TIMEZONE, NTP_DAYLIGHT_SAVING, MINUTES_TIMEZONE);
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
  Serial.println(" minutes");
  return duration;
}

int SchedulerLogic::getPoolTriggerFrequency() {
  int frequency = DEFAULT_POOL_TRIGGER_FREQUENCY;
  Serial.print("trigger frequency: ");
  Serial.print(frequency);
  Serial.println(" hours");
  return frequency;
}

int SchedulerLogic::getPoolDuration() {
  int duration = DEFAULT_ACTIVE_TIME_POOL;
  Serial.print("duration: ");
  Serial.print(duration);
  Serial.println(" minutes");
  return duration;
}
