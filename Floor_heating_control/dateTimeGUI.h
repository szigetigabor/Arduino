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
  itoa(tmpRTC.tHour, char1, 10);
  myGLCD.print("  ", TEXT_HOUR_X, TEXT_PLACE_Y);
  if (tmpRTC.tHour <= 9) {
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
  if (tmpRTC.tMinute <= 9) {
    strcat(char1, "0");
    itoa(tmpRTC.tMinute, tmp, 10);
    strcat(char1, tmp);
  } else { 
    itoa(tmpRTC.tMinute, char1, 10);
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
  itoa(tmpRTC.tYear, char1, 10);
  myGLCD.printNumI(tmpRTC.tYear, TEXT_YEAR_X, TEXT_PLACE_Y);
  myGLCD.setFont(DEFAULT_FONT);            
}

void printMonth() {
  myGLCD.setFont(GroteskBold32x64);
  myGLCD.print(Mon[tmpRTC.tMonth], TEXT_MONTH_X, TEXT_PLACE_Y);
  myGLCD.setFont(DEFAULT_FONT);            
}

void printDay() {
  myGLCD.setFont(GroteskBold32x64);
  char char1[4];
  char1[0] = '\0';
  itoa(tmpRTC.tDay, char1, 10);
  myGLCD.print("  ", TEXT_DAY_X, TEXT_PLACE_Y);
  if (tmpRTC.tDay <= 9) {
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

void initDateTimeSettingGUI() {
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
      // TODO: save date value
      //setRTC("May 11 2011", "0:1:12");
      current_page = PAGE_SETTINGS;
    } else if (pressed_button==but_year_p) {
      tmpRTC.tYear +=1; 
      printYear();
    } else if (pressed_button==but_year_m) {
      tmpRTC.tYear -=1; 
      printYear();
    } else if (pressed_button==but_month_p) {
      if (tmpRTC.tMonth == 12 ) {
        tmpRTC.tMonth = 1;
      } else {
        tmpRTC.tMonth +=1; 
      }
      printMonth();
    } else if (pressed_button==but_month_m) {
      if (tmpRTC.tMonth == 1 ) {
        tmpRTC.tMonth = 12;
      } else {
        tmpRTC.tMonth -=1;
      }
      printMonth();
    } else if (pressed_button==but_day_p) {
      if (tmpRTC.tDay == 31 ) {
        tmpRTC.tDay = 1;
      } else {
        tmpRTC.tDay +=1; 
      }
      printDay();
    } else if (pressed_button==but_day_m) {
      if (tmpRTC.tDay == 1 ) {
        tmpRTC.tDay = 31;
      } else {
        tmpRTC.tDay -=1; 
      } 
      printDay();
    } else if (pressed_button==but_hour_p) {
      if (tmpRTC.tHour == 23 ) {
        tmpRTC.tHour = 0;
      } else {
        tmpRTC.tHour +=1; 
      }
      printHour();
    } else if (pressed_button==but_hour_m) {
      if (tmpRTC.tHour == 0 ) {
        tmpRTC.tHour = 23;
      } else {
        tmpRTC.tHour -=1; 
      } 
      printHour();
    } else if (pressed_button==but_minutes_p) {
      if (tmpRTC.tMinute == 59 ) {
        tmpRTC.tMinute = 0;
      } else {
        tmpRTC.tMinute +=1; 
      }
      printMinute();
    } else if (pressed_button==but_minutes_m) {
      if (tmpRTC.tMinute == 0 ) {
        tmpRTC.tMinute = 59;
      } else {
        tmpRTC.tMinute -=1; 
      } 
      printMinute();
    }
  }
}

#endif
