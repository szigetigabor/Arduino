#ifndef MAIN_GUI
#define MAIN_GUI

#include "Display_includes.h"
#include "RTC_control.h"
#include "dateTimeGUI.h"

extern int pressed_button;
int but_temp, but_settings;

void showDate() {
  char char1[4];
  int size = 16;
  int x = 100;
  int y = 50;
  myGLCD.print("Date", x, y);
  x+=(5*16);

  //print year
  char1[0] = '\0';
  itoa(tmpRTC.tYear, char1, 10);
  myGLCD.printNumI(tmpRTC.tYear, x, y);
  x+=(4*size);
  myGLCD.print(".", x, y);
  x+=size;

  // print month
  myGLCD.print(Mon[tmpRTC.tMonth], x, y);
  x += stringHalfOffset(char1,SMALL);
  x+=size;
  myGLCD.print(".", x, y);
  x+=size;

  //print day
  char1[0] = '\0';
  itoa(tmpRTC.tDay, char1, 10);
  myGLCD.print("  ", x, y);
  if (tmpRTC.tDay <= 9) {
    x += stringHalfOffset(char1,SMALL);
  }
  myGLCD.print(char1, x, y);
  x += size;
  myGLCD.print(".", x, y);

  x = 100 + (6*size);
  y = 80;
  //print hour
  char1[0] = '\0';
  itoa(tmpRTC.tHour, char1, 10);
  myGLCD.print("  ", x, y);
  if (tmpRTC.tHour <= 9) {
    x += stringHalfOffset(char1,SMALL);
  }
  myGLCD.print(char1, x, y);
  x += size;
  myGLCD.print(":", x, y);
  x += size;

  //print minutes
  char1[0] = '\0';
  itoa(tmpRTC.tMinute, char1, 10);
  myGLCD.print("  ", x, y);
  if (tmpRTC.tMinute <= 9) {
    x += stringHalfOffset(char1,SMALL);
  }
  myGLCD.print(char1, x, y);

}

void showOperation() {
  myGLCD.print("Operation informations", 100, 120);
}

void showButtons() {
  int x = 50;
  int y = 250;
  but_temp     = myButtons.addButton( x,     y, 180,  60, "e", BUTTON_SYMBOL);
  but_settings = myButtons.addButton( x+200, y, 180,  60, "w", BUTTON_SYMBOL);
  myButtons.drawButton(but_temp);
  myButtons.drawButton(but_settings);  
}

void initMainGUI() {
  myButtons.deleteAllButtons();
  touched = false;
  prev_page = PAGE_MAIN;
  myGLCD.clrScr();

  //only for debug
  tmpRTC.tYear = 2011;
  tmpRTC.tMonth = 4;
  tmpRTC.tDay = 31;
  tmpRTC.tHour = 1;
  tmpRTC.tMinute = 3;
}

void showMainGUI() {
  if ( current_page != prev_page || touched) {
    initMainGUI();
    showTitle("F$t|s szab{lyoz{s");
    showDate();
    showOperation();
    showButtons();
  }

  //chekck button touching
  if (myTouch.dataAvailable() == true) {
    last_used = millis();
    pressed_button = myButtons.checkButtons();
    if (pressed_button==but_temp) {
      current_page = PAGE_TEMPERATURE;
    } else if (pressed_button==but_settings) {
      current_page = PAGE_SETTINGS;
    }
  }
}

#endif
