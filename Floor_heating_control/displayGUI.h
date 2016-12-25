#ifndef DISPLAY_GUI
#define DISPLAY_GUI

#include "EEPROM_control.h"
#include "Display_includes.h"

#define OPTIONS 5
int delay_button[OPTIONS];
char* labels[] = {"Soha", "10mp", "20mp", "30mp", "1 perc"};
byte used=0;

void showDelayButtons() {
  int x = 150;
  int start = 50;
  for(int i=0; i<OPTIONS; i++) {
    if ( i == used ) {
      myButtons.setButtonColors(VGA_WHITE, VGA_RED, VGA_WHITE, VGA_RED, VGA_BLUE);
      delay_button[i] = myButtons.addButton( x,  start, 180,  45, labels[i], BUTTON_DISABLED);
    } else {
      myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_WHITE, VGA_RED, VGA_BLUE);
      delay_button[i] = myButtons.addButton( x,  start, 180,  45, labels[i]);
    }
    myButtons.drawButton(delay_button[i]);
    start += 55;
  }
}

int calculateActual(int idle) {
  for(int i=0; i<OPTIONS; i++) {
    if ( values[i] == idle) {
      return i;
    }
  }
}

void initDisplaySettingsGUI() {
  myButtons.deleteAllButtons();
  prev_page=PAGE_DISPLAY;
  myGLCD.clrScr();
  used = getSleep();
  idle_max = values[used];
  showTitle("Kijelz# els}t|t\"t|s");
  but_back = showBackButton(); 
  showDelayButtons();
}

void showDisplaySettingGUI() {
  if ( current_page != prev_page || touched) {
    initDisplaySettingsGUI();
    touched = false;
  }

  if (myTouch.dataAvailable() == true) {
    last_used = millis();
    pressed_button = myButtons.checkButtons();
    if (pressed_button==delay_button[0]) {
      if ( debug >= INFO ) {
        Serial.println("off");
      }
      idle_max = values[0];
      used = 0;
      setSleep(used);
      touched = true;
    } else if (pressed_button==delay_button[1]) {
      if ( debug >= INFO ) {
        Serial.println("10s");
      }
      idle_max = values[1];
      used = 1;
      setSleep(used);
      touched = true;
    } else if (pressed_button==delay_button[2]) {
      if ( debug >= INFO ) {
        Serial.println("20s");
      }
      idle_max = values[2];
      used = 2;
      setSleep(used);
      touched = true;
    } else if (pressed_button==delay_button[3]) {
      if ( debug >= INFO ) {
        Serial.println("30s");
      }
      idle_max = values[3];
      used = 3;
      setSleep(used);
      touched = true;
    } else if (pressed_button==delay_button[4]) {
      if ( debug >= INFO ) {
        Serial.println("60s");
      }
      idle_max = values[4];
      used = 4;
      setSleep(used);
      touched = true;
    } else if (pressed_button==but_back) {
      if ( debug >= DEBUG ) {
        Serial.println("BACK");
      }
      current_page = PAGE_SETTINGS;
    }
  }
}
#endif
