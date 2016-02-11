#ifndef SETTINGS_GUI
#define SETTINGS_GUI

#include "Display_includes.h"
#define BUTTON_X_SIZE 200
#define BUTTON_Y_SIZE 60

int but_date, but_alarm, but_night, but_sched, but_disp;

void showSettingsButtons() {
  but_date  = myButtons.addButton( 30,  80, BUTTON_X_SIZE, BUTTON_Y_SIZE, "W", BUTTON_SYMBOL);
  but_alarm = myButtons.addButton( 240, 80, BUTTON_X_SIZE, BUTTON_Y_SIZE, "e", BUTTON_SYMBOL);
  but_sched = myButtons.addButton( 30,  160, BUTTON_X_SIZE, BUTTON_Y_SIZE, "f", BUTTON_SYMBOL);
  but_night = myButtons.addButton( 240, 160, BUTTON_X_SIZE, BUTTON_Y_SIZE, "|", BUTTON_SYMBOL);
  but_disp  = myButtons.addButton( 140, 240, BUTTON_X_SIZE, BUTTON_Y_SIZE, "N", BUTTON_SYMBOL);
  
  myButtons.drawButton(but_date);
  myButtons.drawButton(but_alarm);
  myButtons.drawButton(but_night);
  myButtons.drawButton(but_sched);
  myButtons.drawButton(but_disp);

}
void initSettingsGUI() {
  prev_page = PAGE_SETTINGS;
  touched = false;
  myButtons.deleteAllButtons();
  myGLCD.clrScr();
}
void showSettingsGUI() {
  if ( current_page != prev_page || touched) {
    initSettingsGUI();
    showTitle("Be{ll\"t{sok");
    showSettingsButtons();
    but_back = showBackButton();
  }

  if (myTouch.dataAvailable() == true) {
    last_used = millis();
    pressed_button = myButtons.checkButtons();
    if (pressed_button==but_back) {
      current_page = PAGE_MAIN;
    } else if (pressed_button==but_date) {
      current_page = PAGE_DATE;
    } else if (pressed_button==but_alarm) {
      current_page = PAGE_REQ_TEMP;
    } else if (pressed_button==but_night) {
      current_page = PAGE_REQ_NIGHT_TEMP;
    } else if (pressed_button==but_sched) {
      current_page = PAGE_MODE_TIMING;
    } else if (pressed_button==but_disp) {
      current_page = PAGE_DISPLAY;
    }
  }
}

#endif
