#ifndef SCT_GUI
#define SCT_GUI

#include "Display_includes.h"
#include "sct013.h"

unsigned int last_update, sct_now;

void initSCTGUI() {
  myButtons.deleteAllButtons();
  prev_page=PAGE_SCT;
  
  myGLCD.clrScr();
}

void showLimit() {
  int x = 220;
  int y = 55;
  // show alarm value
  myGLCD.setFont(GroteskBold32x64);
  x += 46;
  y += 120;
  myGLCD.print("Limit:", 0, y);
  myGLCD.printNumF(POWERLIMIT, 0, x, y);
  myGLCD.setFont(GroteskBold32x64);
  myGLCD.print("W", x+128+16, y);
  myGLCD.setFont(hungarian_font_16x16);  
}

void showValue(float current) {
  int x = 100;
  int y = 55;
  myGLCD.print("Akt.fogy.:", x, y);
  myGLCD.printNumF(current, 2, x+180, y);
  myGLCD.print(" W", x+242, y);
}

void showSCTGUI() {
  if ( current_page != prev_page || touched) {
    initSCTGUI();
    touched = false;
  

    showTitle("Kaz{n teljes\"tm|nye");
    but_back = showBackButton();
    showValue(getCurrentPower());
    showLimit();
  }
  sct_now=millis();
  if (last_update +1000 < sct_now && !idle ) {
    showValue(getCurrentPower());
    last_update=millis();
  }
  if (myTouch.dataAvailable() == true) {
    last_used = millis();
    pressed_button = myButtons.checkButtons();
    if (pressed_button==but_back) {
      if ( debug >= DEBUG ) {
        Serial.println("BACK");
      }
      current_page = PAGE_SETTINGS;
    }
  }
}


#endif
