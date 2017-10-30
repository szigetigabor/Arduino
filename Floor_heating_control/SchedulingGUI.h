#ifndef SCHEDULING_GUI
#define SCHEDULING_GUI

#include "EEPROM_control.h"
#include "Display_includes.h"
#include "Scheduling.h"

#define TEXT_MESSAGE_X 115
#define TEXT_MESSAGE_Y 50
#define TEXT_DHOUR_X  50
#define TEXT_DMIN_X   132
#define TEXT_MIDDLE   220
#define TEXT_NHOUR_X  280
#define TEXT_NMIN_X   362


int but_dhour_p, but_dhour_m, but_dminute_p, but_dminute_m;
int but_nhour_p, but_nhour_m, but_nminute_p, but_nminute_m;

byte tmp_dh, tmp_dm, tmp_nh, tmp_nm;


void printHour(int value, int X_PLACE) {
  myGLCD.setFont(GroteskBold32x64);
  char char1[2];
  char1[0] = '\0';
  itoa(value, char1, 10);
  myGLCD.print("  ", X_PLACE, TEXT_PLACE_Y);
  if (value <= 9) {
    myGLCD.print(char1, X_PLACE+stringHalfOffset(char1,LARGE), TEXT_PLACE_Y);
  } else {
    myGLCD.print(char1, X_PLACE, TEXT_PLACE_Y);
  }
  myGLCD.setFont(DEFAULT_FONT);            
}

void printDHour() {
  printHour(tmp_dh, TEXT_DHOUR_X);
}

void printNHour() {
  printHour(tmp_nh, TEXT_NHOUR_X);
}

void printMinute(int value, int X_PLACE) {
  myGLCD.setFont(GroteskBold32x64);
  char char1[4], tmp[3];
  char1[0] = '\0';
  if (value <= 9) {
    strcat(char1, "0");
    itoa(value, tmp, 10);
    strcat(char1, tmp);
  } else { 
    itoa(value, char1, 10);
  }
  myGLCD.print("   ", X_PLACE, TEXT_PLACE_Y);
  myGLCD.print(char1, X_PLACE, TEXT_PLACE_Y);
  myGLCD.setFont(DEFAULT_FONT);            
}

void printDMinute() {
  printMinute(tmp_dm, TEXT_DMIN_X);
}

void printNMinute() {
  printMinute(tmp_nm, TEXT_NMIN_X);
}

void showNightTimeSetting() {
  myButtons.setTextFont(GroteskBoldOwn32x64);

  but_nhour_p    = myButtons.addButton( TEXT_NHOUR_X, BUTTON_UP_Y,   BUTTON_SIZE, BUTTON_SIZE, "+");
  but_nhour_m    = myButtons.addButton( TEXT_NHOUR_X, BUTTON_DOWN_Y, BUTTON_SIZE, BUTTON_SIZE, "-");
  but_nminute_p = myButtons.addButton( TEXT_NMIN_X,  BUTTON_UP_Y,   BUTTON_SIZE, BUTTON_SIZE, "+");
  but_nminute_m = myButtons.addButton( TEXT_NMIN_X,  BUTTON_DOWN_Y, BUTTON_SIZE, BUTTON_SIZE, "-");
  
  myButtons.drawButton(but_nhour_p);
  myButtons.drawButton(but_nhour_m);
  myButtons.drawButton(but_nminute_p);
  myButtons.drawButton(but_nminute_m);

  myButtons.setTextFont(DEFAULT_FONT);
  //myGLCD.setColor(VGA_GRAY);
  myGLCD.setFont(GroteskBold32x64);

  myGLCD.print(":", TEXT_NHOUR_X+59, TEXT_PLACE_Y);
  myGLCD.print("-", TEXT_MIDDLE, TEXT_PLACE_Y);
  printNHour();
  printNMinute();

  myGLCD.setFont(DEFAULT_FONT);
  myGLCD.setColor(DEFAULT_FONT_COLOR);
}

void showDayTimeSetting() {
  myButtons.setTextFont(GroteskBoldOwn32x64);

  but_dhour_p  = myButtons.addButton( TEXT_DHOUR_X,  BUTTON_UP_Y,   BUTTON_SIZE, BUTTON_SIZE, "+");
  but_dhour_m  = myButtons.addButton( TEXT_DHOUR_X,  BUTTON_DOWN_Y, BUTTON_SIZE, BUTTON_SIZE, "-");
  but_dminute_p = myButtons.addButton( TEXT_DMIN_X, BUTTON_UP_Y,   BUTTON_SIZE, BUTTON_SIZE, "+");
  but_dminute_m = myButtons.addButton( TEXT_DMIN_X, BUTTON_DOWN_Y, BUTTON_SIZE, BUTTON_SIZE, "-");
  
  myButtons.drawButton(but_dhour_p);
  myButtons.drawButton(but_dhour_m);
  myButtons.drawButton(but_dminute_p);
  myButtons.drawButton(but_dminute_m);

  myButtons.setTextFont(DEFAULT_FONT);
  myGLCD.setFont(GroteskBold32x64);

  myGLCD.print(":", TEXT_DHOUR_X+59, TEXT_PLACE_Y);
  printDHour();
  printDMinute();

  myGLCD.setFont(DEFAULT_FONT);
}

void initSchedulingSettingsGUI() {
  tmp_dh = day_hour;
  tmp_dm = day_minute;
  tmp_nh = night_hour;
  tmp_nm = night_minute;
  prev_page = PAGE_SCHEDULING;
  touched = false;
  myButtons.deleteAllButtons();
  myGLCD.clrScr();
}

void SaveScheduling() {
  if (tmp_dh != day_hour) {
    day_hour = tmp_dh;
    setDayHour(day_hour);
  }
  if (tmp_dm != day_minute) {
    day_minute = tmp_dm;
    setDayMinute(day_minute);
  }
  if (tmp_nh != night_hour) {
    night_hour = tmp_nh;
    setNightHour(night_hour);
  }
  if (tmp_nm != night_minute) {
    night_minute = tmp_nm;
    setNightMinute(night_minute);
  }

  delay(500);
  // necessary to restart the device because something goes to wrong by the cyclically temperature checking when loaded this GUI. 
  asm volatile ("  jmp 0");
}

void showSchedulingSettingsGUI() {
  if ( current_page != prev_page || touched ) {
    initSchedulingSettingsGUI();
    showTitle("Id#z\"t|s be{ll\"t{s");
    myGLCD.print("Nappali id#szak", TEXT_MESSAGE_X, TEXT_MESSAGE_Y);
    showDayTimeSetting();
    showNightTimeSetting();
    but_back = showBackButton();
  }
 
  if (myTouch.dataAvailable() == true) {
    last_used = millis();
    pressed_button = myButtons.checkButtons();
    if (pressed_button==but_back) {
      SaveScheduling();
      current_page = PAGE_SETTINGS;
    } else if (pressed_button==but_dhour_p) {
      if (tmp_dh == 23 ) {
        tmp_dh = 0;
      } else {
        tmp_dh +=1; 
      }
      printDHour();
    } else if (pressed_button==but_dhour_m) {
      if (tmp_dh == 0 ) {
        tmp_dh = 23;
      } else {
        tmp_dh -=1; 
      } 
      printDHour();
    } else if (pressed_button==but_dminute_p) {
      if (tmp_dm == 59 ) {
        tmp_dm = 0;
      } else {
        tmp_dm +=1; 
      }
      printDMinute();
    } else if (pressed_button==but_dminute_m) {
      if (tmp_dm == 0 ) {
        tmp_dm = 59;
      } else {
        tmp_dm -=1; 
      } 
      printDMinute();
    } else if (pressed_button==but_nhour_p) {
      if (tmp_nh == 23 ) {
        tmp_nh = 0;
      } else {
        tmp_nh +=1; 
      }
      printNHour();
    } else if (pressed_button==but_nhour_m) {
      if (tmp_nh == 0 ) {
        tmp_nh = 23;
      } else {
        tmp_nh -=1; 
      } 
      printNHour();
    } else if (pressed_button==but_nminute_p) {
      if (tmp_nm == 59 ) {
        tmp_nm = 0;
      } else {
        tmp_nm +=1; 
      }
      printNMinute();
    } else if (pressed_button==but_nminute_m) {
      if (tmp_nm == 0 ) {
        tmp_nm = 59;
      } else {
        tmp_nm -=1; 
      } 
      printNMinute();
    }
  }
}

#endif
