#ifndef RESET_GUI
#define RESET_GUI

#include "Display_includes.h"
#define BUTTON_X_SIZE 200
#define BUTTON_Y_SIZE 60

int but_greset;

void showResetButtons() {
  but_reset  = myButtons.addButton( 120,  80, BUTTON_X_SIZE, BUTTON_Y_SIZE, "Reset");
  myButtons.drawButton(but_greset);
}

void initResetGUI() {
  prev_page = PAGE_RESET;
  touched = false;
  myButtons.deleteAllButtons();
  myGLCD.clrScr();
}
void showResetGUI() {
  if ( current_page != prev_page || touched) {
    initResetGUI();
    showTitle("Vissza{ll\"t{s");
    showResetButtons();
    but_back = showBackButton();
  }

  if (myTouch.dataAvailable() == true) {
    last_used = millis();
    pressed_button = myButtons.checkButtons();
    if (pressed_button==but_back) {
      current_page = PAGE_SETTINGS2;
    } else if (pressed_button==but_greset) {
      //setReset();   // not necessary this method, next line is enought to restart a device
      asm volatile ("  jmp 0");
    }
  }
}

#endif
