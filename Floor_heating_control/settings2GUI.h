#ifndef SETTINGS2_GUI
#define SETTINGS2_GUI

#include "Display_includes.h"
#define BUTTON_X_SIZE 200
#define BUTTON_Y_SIZE 60

int but_date, but_reset, but_sct; //but_sched, but_disp;

void showSettings2Buttons() {
  but_date  = myButtons.addButton( 30,  80, BUTTON_X_SIZE, BUTTON_Y_SIZE, "D{tum/Id#");
  but_reset = myButtons.addButton( 240, 80, BUTTON_X_SIZE, BUTTON_Y_SIZE, "Vissza{ll.");
  but_sct = myButtons.addButton( 30,  160, BUTTON_X_SIZE, BUTTON_Y_SIZE, "Kaz{n (W)");
//  but_night = myButtons.addButton( 240, 160, BUTTON_X_SIZE, BUTTON_Y_SIZE, "Esti h#m.");
//  but_disp  = myButtons.addButton( 140, 240, BUTTON_X_SIZE, BUTTON_Y_SIZE, "Kijelz#");
  
  myButtons.drawButton(but_date);
  myButtons.disableButton(but_date);
  myButtons.drawButton(but_reset);
  myButtons.drawButton(but_sct);
//  myButtons.drawButton(but_sched);
//  myButtons.disableButton(but_sched);
//  myButtons.drawButton(but_disp);

}
void initSettings2GUI() {
  prev_page = PAGE_SETTINGS2;
  touched = false;
  myButtons.deleteAllButtons();
  myGLCD.clrScr();
}
void showSettings2GUI() {
  if ( current_page != prev_page || touched) {
    initSettings2GUI();
    showTitle("Be{ll\"t{sok");
    showSettings2Buttons();
    but_back = showBackButton();
  }

  if (myTouch.dataAvailable() == true) {
    last_used = millis();
    pressed_button = myButtons.checkButtons();
    if (pressed_button==but_back) {
      current_page = PAGE_SETTINGS;
    } else if (pressed_button==but_date) {
      current_page = PAGE_DATE;
    } else if (pressed_button==but_alarm) {
      current_page = PAGE_RESET;
    } else if (pressed_button==but_sct) {
      current_page = PAGE_SCT;
    }/* else if (pressed_button==but_sched) {
      current_page = PAGE_MODE_TIMING;
    } else if (pressed_button==but_disp) {
      current_page = PAGE_DISPLAY;
    }*/
  }
}

#endif
