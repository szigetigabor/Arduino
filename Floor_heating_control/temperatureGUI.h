#ifndef TEMPERATURE_GUI
#define TEMPERATURE_GUI

#include "Display_includes.h"
#define X 10
#define Y 60
#define SCALE 3.2

int nappali[2] = {41, 48};
int halo[2]    = {33, 33};
int kamra[2]   = {24, 19};
int konyha[2]  = {24, 27};
int furdo[2]   = {24, 18};
int etkezo[2]  = {33, 31};
int eloter[2]  = {29, 30};
int eloszoba[2]= {44, 14};
int wc[2]      = {13, 14};

void showLayout() {
  int x=X;
  int y=Y;
  
  //nappali
  myGLCD.drawRect(x, y, x+(nappali[0]*SCALE), y+(nappali[1]*SCALE));
  //háló
  x+=nappali[0]*SCALE;
  myGLCD.drawRect(x, y, x+(halo[0]*SCALE), y+(halo[1]*SCALE));
  //kamra
  x+=halo[0]*SCALE;
  myGLCD.drawRect(x, y, x+(kamra[0]*SCALE), y+(kamra[1]*SCALE));
  //konyha
  y+=kamra[1]*SCALE;
  myGLCD.drawRect(x, y, x+(konyha[0]*SCALE), y+(konyha[1]*SCALE));
  //fűrdő
  y+=konyha[1]*SCALE;
  myGLCD.drawRect(x, y, x+(furdo[0]*SCALE), y+(furdo[1]*SCALE));
  //étkező
  x=X+nappali[0]*SCALE;
  y=Y+halo[1]*SCALE;
  myGLCD.drawRect(x, y, x+(etkezo[0]*SCALE), y+(etkezo[1]*SCALE));
  //előtér
  y=Y+((nappali[1]+eloter[1])*SCALE);
  myGLCD.drawRect(x, y-(eloter[1]*SCALE), x-(eloter[0]*SCALE), y);
  //előszoba
  myGLCD.drawRect(x, y, x+(eloszoba[0]*SCALE), y-(eloszoba[1]*SCALE));
  //wc
  x+=(eloszoba[0]*SCALE);
  myGLCD.drawRect(x, y, x+(wc[0]*SCALE), y-(wc[1]*SCALE));
  //tetőtér
  myGLCD.setColor(VGA_RED);
  x=400;
  y=100;
  int delta=55;
  myGLCD.drawLine(x, y, x+delta, y+delta);
  myGLCD.drawLine(x, y, x-delta, y+delta);
  myGLCD.drawLine(x-delta, y+delta, x+delta, y+delta);
  //garázs
  myGLCD.setColor(DEFAULT_FONT_COLOR);
  int high=55;
  myGLCD.drawLine(x-delta, y+delta, x-delta, y+delta+high);
  myGLCD.drawLine(x-delta, y+delta+high, x+delta, y+delta+high);
  myGLCD.drawLine(x+delta, y+delta, x+delta, y+delta+high);
  //garázs ajtó
  myGLCD.drawLine(x-delta+10, y+delta+10, x-delta+10, y+delta+high);
  myGLCD.drawLine(x-delta+10, y+delta+10, x+delta-10, y+delta+10);
  myGLCD.drawLine(x+delta-10, y+delta+10, x+delta-10, y+delta+high);
  //pince
  int phigh=30;
  myGLCD.drawLine(x-delta+10, y+delta+high+phigh, x-delta+10, y+delta+high);
  myGLCD.drawLine(x-delta+10, y+delta+high+phigh, x+delta-10, y+delta+high+phigh);
  myGLCD.drawLine(x+delta-10, y+delta+high+phigh, x+delta-10, y+delta+high);

}

void putTemp(float value, int x, int y) {
  myGLCD.printNumF(value, 1, x, y, '.', 4);
}

void showTemp() {
  //nappali
  putTemp(last_temp[5], X+30, Y+(nappali[1]*SCALE/2));
  //háló
  putTemp(last_temp[6], X+150, Y+(halo[1]*SCALE/2));
  //kamra
  putTemp(last_temp[9], X+244, Y+(kamra[1]*SCALE/2));
  //konyha
  putTemp(last_temp[0], X+244, Y+((kamra[1]+(konyha[1]/2))*SCALE));
  //fürdő
  putTemp(last_temp[2], X+244, Y+((kamra[1]+konyha[1]+(furdo[1]/3))*SCALE));
  //étkező
  putTemp(last_temp[1], X+150, Y+((halo[1]+(etkezo[1]/2))*SCALE));
  //előtér
  putTemp(last_temp[7], X+60, Y+((nappali[1]+(eloter[1]/2))*SCALE));
  //előszoba
  putTemp(last_temp[3], X+150, Y+((halo[1]+etkezo[1]+(eloszoba[1]/3))*SCALE));
  //wc
  putTemp(last_temp[4], X+273, Y+((halo[1]+etkezo[1]+(eloszoba[1]/3))*SCALE));
  //pince
  putTemp(last_temp[12], 368, 218);
  //garázs
  putTemp(last_temp[11], 368, 185);
  //tetőtér
  putTemp(last_temp[10], 368, 135);
  //külső
  putTemp(last_temp[8], 360, 80);

  myGLCD.setColor(DEFAULT_FONT_COLOR);
}

void initTemperatures() {
  prev_page = PAGE_TEMPERATURE;
  touched =  false;
  myButtons.deleteAllButtons();
  myGLCD.clrScr();
}

void showTemperatures() {
  if ( current_page != prev_page || touched) {
    initTemperatures();
    showTitle("H#m|rs|kletek");
    but_back = showBackButton();
    showLayout();
    showTemp();
  }

  if (myTouch.dataAvailable() == true) {
    last_used = millis();
    pressed_button = myButtons.checkButtons();
    Serial.println("checked TOUCHED!!!!!");
    if (pressed_button==but_back) {
      current_page = 1;
    }
  }
}
#endif
