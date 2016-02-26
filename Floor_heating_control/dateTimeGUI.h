#ifndef DATETIME_GUI
#define DATETIME_GUI

#include "EEPROM_control.h"
#include "Display_includes.h"
#include "RTC_control.h"

#define TEXT_YEAR_X  5
#define TEXT_MONTH_X 138
#define TEXT_DAY_X   239
#define TEXT_HOUR_X  318
#define TEXT_MIN_X   400

#define BUTTON_SIZE  66

#define BUTTON_UP_Y  81
#define TEXT_PLACE_Y 161
#define BUTTON_DOWN_Y 240

int but_hour_p, but_hour_m, but_minutes_p, but_minutes_m;
int but_year_p, but_year_m, but_month_p, but_month_m, but_day_p, but_day_m;

#define SMALL false
#define LARGE true

int stringHalfOffset (char *myString, bool font=false)               //returns half lenght of string in pixels
{
  int chLen;
  
  if (font)                                   //large font
    chLen = (strlen(myString) * 16) -1;
  else                                         //small font
    chLen = (strlen(myString) * 8) -1;
  return chLen;
}

void printHour() {
  myGLCD.setFont(GroteskBold32x64);
  char char1[2];
  char1[0] = '\0';
  itoa(modRTC.tHour, char1, 10);
  myGLCD.print("  ", TEXT_HOUR_X, TEXT_PLACE_Y);
  if (modRTC.tHour <= 9) {
    myGLCD.print(char1, TEXT_HOUR_X+stringHalfOffset(char1,LARGE), TEXT_PLACE_Y);
  } else {
    myGLCD.print(char1, TEXT_HOUR_X, TEXT_PLACE_Y);
  }
  myGLCD.setFont(DEFAULT_FONT);            
}

void printMinute() {
  myGLCD.setFont(GroteskBold32x64);
  char char1[4], tmp[3];
  char1[0] = '\0';
  if (modRTC.tMinute <= 9) {
    strcat(char1, "0");
    itoa(modRTC.tMinute, tmp, 10);
    strcat(char1, tmp);
  } else { 
    itoa(modRTC.tMinute, char1, 10);
  }
  myGLCD.print("   ", TEXT_MIN_X, TEXT_PLACE_Y);
  myGLCD.print(char1, TEXT_MIN_X, TEXT_PLACE_Y);
  myGLCD.setFont(DEFAULT_FONT);            
}

void showTimeSetting() {
  //myGLCD.print("Id#:", 10, 81);

  myButtons.setTextFont(GroteskBoldOwn32x64);

  but_hour_p    = myButtons.addButton( TEXT_HOUR_X, BUTTON_UP_Y,   BUTTON_SIZE, BUTTON_SIZE, "+");
  but_hour_m    = myButtons.addButton( TEXT_HOUR_X, BUTTON_DOWN_Y, BUTTON_SIZE, BUTTON_SIZE, "-");
  but_minutes_p = myButtons.addButton( TEXT_MIN_X,  BUTTON_UP_Y,   BUTTON_SIZE, BUTTON_SIZE, "+");
  but_minutes_m = myButtons.addButton( TEXT_MIN_X,  BUTTON_DOWN_Y, BUTTON_SIZE, BUTTON_SIZE, "-");
  
  myButtons.drawButton(but_hour_p);
  myButtons.drawButton(but_hour_m);
  myButtons.drawButton(but_minutes_p);
  myButtons.drawButton(but_minutes_m);

  myButtons.setTextFont(DEFAULT_FONT);
  //myGLCD.setColor(VGA_GRAY);
  myGLCD.setFont(GroteskBold32x64);

  myGLCD.print(":", TEXT_HOUR_X+59, TEXT_PLACE_Y);
  printHour();
  printMinute();

  myGLCD.setFont(DEFAULT_FONT);
  myGLCD.setColor(DEFAULT_FONT_COLOR);
}

void printYear() {
  myGLCD.setFont(GroteskBold32x64);
  char char1[4];
  char1[0] = '\0';
  itoa(modRTC.tYear, char1, 10);
  myGLCD.printNumI(modRTC.tYear, TEXT_YEAR_X, TEXT_PLACE_Y);
  myGLCD.setFont(DEFAULT_FONT);            
}

void printMonth() {
  myGLCD.setFont(GroteskBold32x64);
  myGLCD.print(Mon[modRTC.tMonth], TEXT_MONTH_X, TEXT_PLACE_Y);
  myGLCD.setFont(DEFAULT_FONT);            
}

void printDay() {
  myGLCD.setFont(GroteskBold32x64);
  char char1[4];
  char1[0] = '\0';
  itoa(modRTC.tDay, char1, 10);
  myGLCD.print("  ", TEXT_DAY_X, TEXT_PLACE_Y);
  if (modRTC.tDay <= 9) {
    myGLCD.print(char1, TEXT_DAY_X+stringHalfOffset(char1,LARGE), TEXT_PLACE_Y);
  } else {
    myGLCD.print(char1, TEXT_DAY_X, TEXT_PLACE_Y);
  }
  myGLCD.setFont(DEFAULT_FONT);            
}

void showDateSetting() {
  //myGLCD.print("D{tum:", 10, 170);
  myButtons.setTextFont(GroteskBoldOwn32x64);

  but_year_p  = myButtons.addButton( TEXT_YEAR_X+32,  BUTTON_UP_Y,   BUTTON_SIZE, BUTTON_SIZE, "+");
  but_year_m  = myButtons.addButton( TEXT_YEAR_X+32,  BUTTON_DOWN_Y, BUTTON_SIZE, BUTTON_SIZE, "-");
  but_month_p = myButtons.addButton( TEXT_MONTH_X+12, BUTTON_UP_Y,   BUTTON_SIZE, BUTTON_SIZE, "+");
  but_month_m = myButtons.addButton( TEXT_MONTH_X+12, BUTTON_DOWN_Y, BUTTON_SIZE, BUTTON_SIZE, "-");
  but_day_p   = myButtons.addButton( TEXT_DAY_X,      BUTTON_UP_Y,   BUTTON_SIZE, BUTTON_SIZE, "+");
  but_day_m   = myButtons.addButton( TEXT_DAY_X,      BUTTON_DOWN_Y, BUTTON_SIZE, BUTTON_SIZE, "-");
  
  myButtons.drawButton(but_year_p);
  myButtons.drawButton(but_year_m);
  myButtons.drawButton(but_month_p);
  myButtons.drawButton(but_month_m);
  myButtons.drawButton(but_day_p);
  myButtons.drawButton(but_day_m);

  myButtons.setTextFont(DEFAULT_FONT);
  myGLCD.setFont(GroteskBold32x64);

  printYear();         
  printMonth();
  printDay();

  myGLCD.setFont(DEFAULT_FONT);
}

void SaveTime() {
  tmpRTC = modRTC;

  tmElements_t newtime;
  newtime.Hour   = tmpRTC.tHour;
  newtime.Minute = tmpRTC.tMinute;
  //newtime.Wday   = tmpRTC.tDow;
  newtime.Day    = tmpRTC.tDay;
  newtime.Month  = tmpRTC.tMonth;
  newtime.Year   = CalendarYrToTm(tmpRTC.tYear);
  setRTC(newtime);
}

void initDateTimeSettingGUI() {
  modRTC = tmpRTC;
  prev_page = PAGE_DATE;
  touched = false;
  myButtons.deleteAllButtons();
  myGLCD.clrScr();
}

void showDateTimeSettingGUI() {
  if ( current_page != prev_page || touched ) {
    initDateTimeSettingGUI();
    showTitle("D{tum |s id# be{ll\"t{s");
    showDateSetting();
    showTimeSetting();
    but_back = showBackButton();
  }
 
  if (myTouch.dataAvailable() == true) {
    last_used = millis();
    pressed_button = myButtons.checkButtons();
    if (pressed_button==but_back) {
      SaveTime();
      current_page = PAGE_SETTINGS;
    } else if (pressed_button==but_year_p) {
      modRTC.tYear +=1; 
      printYear();
    } else if (pressed_button==but_year_m) {
      modRTC.tYear -=1; 
      printYear();
    } else if (pressed_button==but_month_p) {
      if (modRTC.tMonth == 12 ) {
        modRTC.tMonth = 1;
      } else {
        modRTC.tMonth +=1; 
      }
      printMonth();
    } else if (pressed_button==but_month_m) {
      if (modRTC.tMonth == 1 ) {
        modRTC.tMonth = 12;
      } else {
        modRTC.tMonth -=1;
      }
      printMonth();
    } else if (pressed_button==but_day_p) {
      if (modRTC.tDay == 31 ) {
        modRTC.tDay = 1;
      } else {
        modRTC.tDay +=1; 
      }
      printDay();
    } else if (pressed_button==but_day_m) {
      if (modRTC.tDay == 1 ) {
        modRTC.tDay = 31;
      } else {
        modRTC.tDay -=1; 
      } 
      printDay();
    } else if (pressed_button==but_hour_p) {
      if (modRTC.tHour == 23 ) {
        modRTC.tHour = 0;
      } else {
        modRTC.tHour +=1; 
      }
      printHour();
    } else if (pressed_button==but_hour_m) {
      if (modRTC.tHour == 0 ) {
        modRTC.tHour = 23;
      } else {
        modRTC.tHour -=1; 
      } 
      printHour();
    } else if (pressed_button==but_minutes_p) {
      if (modRTC.tMinute == 59 ) {
        modRTC.tMinute = 0;
      } else {
        modRTC.tMinute +=1; 
      }
      printMinute();
    } else if (pressed_button==but_minutes_m) {
      if (modRTC.tMinute == 0 ) {
        modRTC.tMinute = 31;
      } else {
        modRTC.tMinute -=1; 
      } 
      printMinute();
    }
  }
}

#endif
