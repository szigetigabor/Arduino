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


void showTime() {
  char char1[4], tmp[3];

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
  myGLCD.setColor(DEFAULT_FONT_COLOR );
  myGLCD.setFont(GroteskBold32x64);
  
  //print hour
  char1[0] = '\0';
  itoa(tmpRTC.tHour, char1, 10);
  myGLCD.print("   ", TEXT_HOUR_X, TEXT_PLACE_Y);
  if (tmpRTC.tHour <= 9) {
    myGLCD.print(char1, TEXT_HOUR_X+stringHalfOffset(char1,LARGE), TEXT_PLACE_Y);
  } else {
    myGLCD.print(char1, TEXT_HOUR_X, TEXT_PLACE_Y);
  }
  myGLCD.print(":", TEXT_HOUR_X+59, TEXT_PLACE_Y);

  //print minutes
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

void showDateSetting() {
  char char1[4], tmp[3];

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
  //print year
  char1[0] = '\0';
  itoa(tmpRTC.tYear, char1, 10);
  myGLCD.printNumI(tmpRTC.tYear, TEXT_YEAR_X, TEXT_PLACE_Y);             

  // print month
  myGLCD.print(Mon[tmpRTC.tMonth], TEXT_MONTH_X, TEXT_PLACE_Y);

  //print day
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

void showDateTimeSettingGUI() {
  myButtons.deleteAllButtons();
  tmpRTC = {14, 9, 1, 18, 1, 2016};
  showTitle("D{tum |s id# be{ll\"t{s", 170);
  showTime();
  showDateSetting();
  but_back = showBackButton();

  
  if (myTouch.dataAvailable() == true) {
    pressed_button = myButtons.checkButtons();
    if (pressed_button==but_back) {
      current_page = 3;
    }
  }
}

#endif
