#ifndef MODE_GUI
#define MODE_GUI

#include "Display_includes.h"
#define BUTTON_X_SIZE 200
#define BUTTON_Y_SIZE 60

int but_on, but_off, but_msched;

void UpdateButtons() {
  myButtons.enableButton(but_on, true);
  myButtons.enableButton(but_off, true);
  myButtons.enableButton(but_msched, true);
  if ( current_mode == MODE_ON ) {
    myButtons.disableButton(but_on, true);
  } else if ( current_mode == MODE_OFF ) {
    myButtons.disableButton(but_off, true);
  } else if ( current_mode == MODE_SCHEDULING ) {
    myButtons.disableButton(but_msched, true);
  }  
}

void showModeButtons() {
  but_off  = myButtons.addButton( 30,  80, BUTTON_X_SIZE, BUTTON_Y_SIZE, "Ki");
  but_on = myButtons.addButton( 240, 80, BUTTON_X_SIZE, BUTTON_Y_SIZE, "Be");
  but_msched = myButtons.addButton( 30,  160, BUTTON_X_SIZE, BUTTON_Y_SIZE, "Id#z\"t|s");
  
  myButtons.drawButton(but_on);
  myButtons.drawButton(but_off);
  myButtons.drawButton(but_msched);
  UpdateButtons();
}

void initModeGUI() {
  prev_page = PAGE_MODE;
  touched = false;
  myButtons.deleteAllButtons();
  myGLCD.clrScr();
}
void showModeGUI() {
  if ( current_page != prev_page || touched) {
    initModeGUI();
    showTitle("^zemmo'd Be{ll\"t{sok");
    showModeButtons();
    but_back = showBackButton();
  }

  if (myTouch.dataAvailable() == true) {
    last_used = millis();
    pressed_button = myButtons.checkButtons();
    if (pressed_button==but_back) {
      current_page = PAGE_SETTINGS;
    } else if (pressed_button==but_on) {
      current_mode = MODE_ON;
      setMode(MODE_ON);
    } else if (pressed_button==but_off) {
      current_mode = MODE_OFF;
      setMode(MODE_OFF);
    } else if (pressed_button==but_msched) {
      current_mode = MODE_SCHEDULING;
      setMode(MODE_SCHEDULING);
    }
    UpdateButtons();
  }
}

#endif
