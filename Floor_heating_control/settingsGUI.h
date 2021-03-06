#ifndef SETTINGS_GUI
#define SETTINGS_GUI

#include "Display_includes.h"
#define BUTTON_X_SIZE 200
#define BUTTON_Y_SIZE 60

int but_mode, but_alarm, but_night, but_sched, but_disp, but_sett;

void showSettingsButtons() {
//  but_date  = myButtons.addButton( 30,  80, BUTTON_X_SIZE, BUTTON_Y_SIZE, "W", BUTTON_SYMBOL);
//  but_alarm = myButtons.addButton( 240, 80, BUTTON_X_SIZE, BUTTON_Y_SIZE, "e", BUTTON_SYMBOL);
//  but_sched = myButtons.addButton( 30,  160, BUTTON_X_SIZE, BUTTON_Y_SIZE, "f", BUTTON_SYMBOL);
//  but_night = myButtons.addButton( 240, 160, BUTTON_X_SIZE, BUTTON_Y_SIZE, "|", BUTTON_SYMBOL);
//  but_disp  = myButtons.addButton( 140, 240, BUTTON_X_SIZE, BUTTON_Y_SIZE, "N", BUTTON_SYMBOL);
  but_mode  = myButtons.addButton( 30,  80, BUTTON_X_SIZE, BUTTON_Y_SIZE, "^zemm&d");
  but_alarm = myButtons.addButton( 240, 80, BUTTON_X_SIZE, BUTTON_Y_SIZE, "Nappali h#m.");
  but_sched = myButtons.addButton( 30,  160, BUTTON_X_SIZE, BUTTON_Y_SIZE, "Id#z\"t|s");
  but_night = myButtons.addButton( 240, 160, BUTTON_X_SIZE, BUTTON_Y_SIZE, "Esti h#m.");
  but_disp  = myButtons.addButton( 30, 240, BUTTON_X_SIZE, BUTTON_Y_SIZE, "Kijelz#");
  but_sett  = myButtons.addButton( 240, 240, BUTTON_X_SIZE, BUTTON_Y_SIZE, "Tov{bb");
  
  myButtons.drawButton(but_mode);
  myButtons.drawButton(but_alarm);
  myButtons.drawButton(but_night);
  myButtons.drawButton(but_sched);
  myButtons.drawButton(but_disp);
  myButtons.drawButton(but_sett);

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
    } else if (pressed_button==but_mode) {
      current_page = PAGE_MODE;
    } else if (pressed_button==but_alarm) {
      current_page = PAGE_REQ_TEMP;
    } else if (pressed_button==but_night) {
      current_page = PAGE_REQ_NIGHT_TEMP;
    } else if (pressed_button==but_sched) {
      current_page = PAGE_SCHEDULING;
    } else if (pressed_button==but_disp) {
      current_page = PAGE_DISPLAY;
    } else if (pressed_button==but_sett) {
      current_page = PAGE_SETTINGS2;
    }
  }
}

#endif
