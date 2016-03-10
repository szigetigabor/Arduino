#ifndef CURRENTS_GUI
#define CURRENTS_GUI

#include "Display_includes.h"
#include "sct013.h"


void initCurrentsGUI() {
  prev_page = PAGE_CURRENTS;
  touched = false;
  myButtons.deleteAllButtons();
  myGLCD.clrScr();
}

void showCurrents() {
  myGLCD.printNumF(getCurrentPower(),2, 100, 100);
  delay(1000);
}

void showCurrentsGUI() {
  if ( current_page != prev_page || touched) {
    initCurrentsGUI();
    showTitle("Aktualit{sok");
    but_back = showBackButton();
  }
showCurrents();
  if (myTouch.dataAvailable() == true) {
    last_used = millis();
    pressed_button = myButtons.checkButtons();
    if (pressed_button==but_back) {
      current_page = PAGE_MAIN;
    } /*else if (pressed_button==but_date) {
      current_page = PAGE_DATE;
    } */
  }
}

#endif
