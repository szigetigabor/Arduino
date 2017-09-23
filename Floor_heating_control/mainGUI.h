#ifndef MAIN_GUI
#define MAIN_GUI

#include "Display_includes.h"
#include "RTC_control.h"
#include "dateTimeGUI.h"

extern int pressed_button;
int but_temp, but_settings;

#define RADIUS   15
#define DISTANCE 45

void showDate() {
  char char1[4];
  int size = 16;
  int charNR = 1;
  int x = 70;
  int y = 50;

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
  charNR = 2;
  if (tmpRTC.tDay <= 9) {
    x += stringHalfOffset(char1,SMALL);
    charNR = 1;
  }
  myGLCD.print(char1, x, y);
  x += charNR*size;
  myGLCD.print(".", x, y);
  x+=(4*size);

  //print hour
  char1[0] = '\0';
  itoa(tmpRTC.tHour, char1, 10);
  myGLCD.print("  ", x, y);
  charNR = 2;
  if (tmpRTC.tHour <= 9) {
    x += stringHalfOffset(char1,SMALL);
    charNR = 1;
  }
  myGLCD.print(char1, x, y);
  x += charNR*size;
  myGLCD.print(":", x, y);
  x += size;

  //print minutes
  char1[0] = '\0';
  itoa(tmpRTC.tMinute, char1, 10);
  myGLCD.print("  ", x, y);
  if (tmpRTC.tMinute <= 9) {
    myGLCD.print("0", x, y);
    x += stringHalfOffset(char1,LARGE);
  }
  myGLCD.print(char1, x, y);

}

void drawCircle(int x, int y, int radius) {
  myGLCD.drawCircle(x, y, radius +2);
}

void drawfilledCircle(int x, int y, int radius, int color) {
  myGLCD.setColor(color);
  myGLCD.fillCircle(x, y, radius);
  myGLCD.setColor(DEFAULT_FONT_COLOR);
}

void checkState(int x, int y, bool condition) {
  if ( condition ) {
    drawfilledCircle(x, y, RADIUS, VGA_RED);
  } else {
    drawfilledCircle(x, y, RADIUS, VGA_BLACK);
  }
}

void drawBackground() {
  //myGLCD.print("Operation informations", 100, 120);
  int x, y;

  x = 50;
  y = 180;
  myGLCD.setColor(VGA_SILVER);
  //myGLCD.drawRect(x-RADIUS-5, y-RADIUS-5, x+200, y+RADIUS+5);
  myGLCD.fillRect(x-RADIUS-3, y-RADIUS-3, x+200, y+RADIUS+3);
  // toilett
  drawCircle(x, y, RADIUS +1);
  x+=DISTANCE;
  // ante-room
  drawCircle(x, y, RADIUS +1);
  x+=DISTANCE;
  // diner
  drawCircle(x, y, RADIUS +1);
  x+=DISTANCE;
  // bath
  drawCircle(x, y, RADIUS +1);
  x+=DISTANCE;
  // kitchen
  drawCircle(x, y, RADIUS +1);
  x+=1.8*DISTANCE;
  //heating pump
  myGLCD.print("M", x-8, y-33);
  drawCircle(x, y, RADIUS +1);

  x=415;
  y=150;
  //boiler
  myGLCD.fillRoundRect(x+8, y-50, x+28, y-55);
  myGLCD.drawRect(x-RADIUS-8, y+RADIUS+8, x+60, y-100);
  drawCircle(x, y, RADIUS +1);
}

void showOperation() {
  int x, y;
  x = 50;
  y = 180;
  // toilett
  checkState(x, y, !relay_status[0]);
  x+=DISTANCE;
  // ante-room
  checkState(x, y, !relay_status[1]);
  x+=DISTANCE;
  // diner
  checkState(x, y, !relay_status[2]);
  x+=DISTANCE;
  // bath
  checkState(x, y, !relay_status[3]);
  x+=DISTANCE;
  // kitchen
  checkState(x, y, !relay_status[4]);
  x+=1.8*DISTANCE;
  //heating pump
  checkState(x, y, !relay_status[5]);
  
  x=415;
  y=150;
  checkState(x, y, boilerIsRunning);
}

void showButtons() {
  int x = 50;
  int y = 250;
//  but_temp     = myButtons.addButton( x,     y, 180,  60, "e", BUTTON_SYMBOL);
//  but_settings = myButtons.addButton( x+200, y, 180,  60, "w", BUTTON_SYMBOL);
  but_temp     = myButtons.addButton( x-25,     y, 210,  60, "H#m|rs|kletek");
  but_settings = myButtons.addButton( x+200, y, 180,  60, "Be{ll\"t{sok");
  myButtons.drawButton(but_temp);
  myButtons.drawButton(but_settings);  
}

void initMainGUI() {
  myButtons.deleteAllButtons();
  touched = false;
  prev_page = PAGE_MAIN;
  myGLCD.clrScr();
}

void showMainGUI() {
  if ( current_page != prev_page || touched) {
    initMainGUI();
    showTitle("F$t|s szab{lyoz{s");
    showDate();
    drawBackground();
    showOperation();
    showButtons();
  }
  if ( prevRTC.tMinute != tmpRTC.tMinute && !idle ) {
//    showDate();
  }
  if ( !idle ) {
    showOperation();
  }

  //check button touching
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
