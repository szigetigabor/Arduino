#ifndef ALARM_GUI
#define ALARM_GUI

#include "EEPROM_control.h"
#include "Display_includes.h"




//extern uint8_t SmallFont[];
//extern uint8_t BigFont[];
//extern uint8_t GroteskBold32x64[];
//extern uint8_t franklingothic_normal[];
//extern uint8_t hungarian_font_16x16[];
//extern word DEFAULT_BACKGROUND;
//extern word DEFAULT_FONT_COLOR;
extern float last_temp[];

int room_button[4];
int but_back, but_up, but_down, pressed_button;
float alarm;

void initAlarmSettingsGUI(int sensor_index=0) {
  myButtons.deleteAllButtons();
  alarm=0;
  if ( alarm == 0 ) {
    int addr = getSensorAddress(sensor_index);
    TempSensorData value;
    EEPROM.get(addr, value);
    alarm=value.alarm;
  }
  Serial.print("ALARM GUI (init):");

  myGLCD.clrScr();
}

/*void showTitle( char title[20], int offset ) {
  myGLCD.setColor(VGA_BLUE);
  myGLCD.fillRect(0, 0, 480, 35);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.setBackColor(VGA_BLUE);
  myGLCD.print(title, 240-offset, 10);

  //set default color
  myGLCD.setColor(DEFAULT_FONT_COLOR);
  myGLCD.setBackColor(DEFAULT_BACKGROUND);

  //back button
  but_back = myButtons.addButton( 1, 1, 50,  50, "9", BUTTON_SYMBOL);
  myButtons.drawButton(but_back);
}*/

void showRooms(int used) {
  int start = 50;
  TempSensorData value;
  for(int i=0; i<ROOMS; i++) {
    int addr = getSensorAddress(i);
    
    EEPROM.get(addr, value); 
    if ( i == used ) {
      room_button[i] = myButtons.addButton( 290,  start, 180,  50, value.name, BUTTON_DISABLED);
    } else {
      room_button[i] = myButtons.addButton( 290,  start, 180,  50, value.name);
    }
    myButtons.drawButton(room_button[i]);
    start += 60;
  }
}

void showSettings(float alarm, int index) {
  int x = 30;
  int y = 55;
  myGLCD.print("Akt.h#m.:", 10, y);
  last_temp[index] = 23.456;
  myGLCD.printNumF(last_temp[index], 2, x+140, y);
  myGLCD.print("%C", x+220, y);

  // up and down buttons
  but_up   = myButtons.addButton( 20,  y+35, 250,  60, "up");
  but_down = myButtons.addButton( 20,  y+200, 250,  60, "down");
  myButtons.drawButton(but_up);
  myButtons.drawButton(but_down);

  // show alarm value
  myGLCD.setFont(GroteskBold32x64);
  x  = 50;
  y += 120;
  myGLCD.printNumF(alarm, 2, x, y);
  myGLCD.setFont(franklingothic_normal);
  myGLCD.print("O", x+160, y);
  myGLCD.setFont(GroteskBold32x64);
  myGLCD.print("C", x+160+16, y);
  myGLCD.setFont(hungarian_font_16x16);
}

void showAlarmSettingGUI(int sensor_index=0) {
  
  initAlarmSettingsGUI(sensor_index);

  showTitle("H#m|rs|klet be{ll\"t{s", 170);
  showRooms(sensor_index);
  showSettings(alarm, sensor_index);

  Serial.print("ALARM GUI (loop):");
  if (myTouch.dataAvailable() == true) {
    pressed_button = myButtons.checkButtons();
    if (pressed_button==room_button[0]) {
      alarm = 0;
      initAlarmSettingsGUI(0);
    } else if (pressed_button==room_button[1]) {
      alarm = 0;
      initAlarmSettingsGUI(1);
    }
  }
  Serial.println("ALARM GUI (sleep):");
}

#endif
