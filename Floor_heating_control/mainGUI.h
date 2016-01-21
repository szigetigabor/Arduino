#ifndef MAIN_GUI
#define MAIN_GUI

#include "Display_includes.h"

extern int pressed_button;
int but_temp, but_settings;

void showDate() {
  myGLCD.print("Date", 100, 50);
}

void showOperation() {
  myGLCD.print("Opeation informations", 100, 80);
}

void showButtons() {
  int x = 50;
  int y = 250;
  but_temp     = myButtons.addButton( x,     y, 180,  60, "e", BUTTON_SYMBOL);
  but_settings = myButtons.addButton( x+200, y, 180,  60, "w", BUTTON_SYMBOL);
  myButtons.drawButton(but_temp);
  myButtons.drawButton(but_settings);  
}

void showMainGUI() {
  showTitle("F$t|s szab{lyoz{s");
  showDate();
  showOperation();
  showButtons();

  //chekck button touching
  if (myTouch.dataAvailable() == true) {
    pressed_button = myButtons.checkButtons();
    if (pressed_button==but_temp) {
      current_page = PAGE_TEMPERATURE;
    } else if (pressed_button==but_settings) {
      current_page = PAGE_SETTINGS;
    }
  }
}

#endif
