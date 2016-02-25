#ifndef RTC_CONTROL
#define RTC_CONTROL

#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>


char *Day[] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
char *Mon[] = {"","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

struct RTC_T
{  
  int tHour;
  int tMinute;
  int tDow;
  int tDay;
  int tMonth;
  int tYear;
} tmpRTC, prevRTC;

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}

void readRTC() {
  prevRTC = tmpRTC;
  tmElements_t tm;
  if (RTC.read(tm)) {
    Serial.print("Ok, Time = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(Mon[tm.Month]);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.write(' ');
    Serial.print(Day[tm.Wday]);
    Serial.println();
    tmpRTC.tHour   = tm.Hour;
    tmpRTC.tMinute = tm.Minute;
    tmpRTC.tDow    = tm.Wday;
    tmpRTC.tDay    = tm.Day;
    tmpRTC.tMonth  = tm.Month;
    tmpRTC.tYear   = tmYearToCalendar(tm.Year);
  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
  }
}

bool getTime(const char *str, tmElements_t& tm)
{
  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}

bool getDate(const char *str,tmElements_t& tm)
{
  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
  for (monthIndex = 1; monthIndex < 13; monthIndex++) {
    if (strcmp(Month, Mon[monthIndex]) == 0) break;
  }
  if (monthIndex >= 13) return false;
  //tm.Wday = dayOfWeek(now());
  tm.Day = Day;
  tm.Month = monthIndex;
  Serial.println(Year);
  tm.Year = CalendarYrToTm(Year);
  return true;
}

void setRTC(char* _date = "", char* _time = "") {
  tmElements_t tm;
  bool parse=false;
  bool config=false;

  if (getDate(_date, tm) && getTime(_time,tm)) {
    parse = true;
    // and configure the RTC with this info
    if (RTC.write(tm)) {
      tm.Wday = dayOfWeek(RTC.get())-1;
      RTC.write(tm);  // update only day of week
      config = true;
    }
  }
  delay(200);
  if (parse && config) {
    Serial.print("DS1307 configured Time=");
    Serial.print(_time);
    Serial.print(", Date=");
    Serial.println(_date);
  } else if (parse) {
    Serial.println("DS1307 Communication Error :-{");
    Serial.println("Please check your circuitry");
  } else {
    Serial.print("Could not parse info from the compiler, Time=\"");
    Serial.print(_time);
    Serial.print("\", Date=\"");
    Serial.print(_date);
    Serial.println("\"");
  }
}

void setRTCatCompilation() {
  // get the date and time the compiler was run
  setRTC(__DATE__, __TIME__);
}
#endif
