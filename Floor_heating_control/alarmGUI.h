#ifndef ALARM_GUI
#define ALARM_GUI

#include "EEPROM_control.h"
#include "Display_includes.h"

extern float last_temp[];

int room_button[ROOMS];
int but_up, but_down;
float alarm;
bool  night_mode;

void initAlarmSettingsGUI(int sensor_index=0, bool night=false) {
  myButtons.deleteAllButtons();
  alarm=0;
  night_mode = night;
  if ( alarm == 0 ) {
    int addr = getSensorAddress(sensor_index);
    TempSensorData value;
    EEPROM.get(addr, value);
    if ( night_mode) {
      alarm=value.night;
    } else {
      alarm=value.alarm;
    }
  }
  Serial.print("ALARM GUI (init):");

  myGLCD.clrScr();
}

void showRooms(int used) {
  int x = 10;
  int start = 55;
  TempSensorData value;
  for(int i=0; i<ROOMS; i++) {
    int addr = getSensorAddress(i);
    
    EEPROM.get(addr, value); 
    if ( i == used ) {
      room_button[i] = myButtons.addButton( x,  start, 180,  45, value.name, BUTTON_DISABLED);
    } else {
      room_button[i] = myButtons.addButton( x,  start, 180,  45, value.name);
    }
    myButtons.drawButton(room_button[i]);
    start += 55;
  }
}

void showSettings(float alarm, int index) {
  int x = 220;
  int y = 55;
  myGLCD.print("Akt.h#m.:", x, y);
  last_temp[index] = 23.456;
  myGLCD.printNumF(last_temp[index], 2, x+140, y);
  myGLCD.print("%C", x+220, y);

  // up and down buttons
  but_up   = myButtons.addButton( x,  y+35, 250,  60, "up");
  but_down = myButtons.addButton( x,  y+200, 250,  60, "down");
  myButtons.drawButton(but_up);
  myButtons.drawButton(but_down);

  // show alarm value
  myGLCD.setFont(GroteskBold32x64);
  x += 30;
  y += 120;
  myGLCD.printNumF(alarm, 2, x, y);
  myGLCD.setFont(franklingothic_normal);
  myGLCD.print("O", x+160, y);
  myGLCD.setFont(GroteskBold32x64);
  myGLCD.print("C", x+160+16, y);
  myGLCD.setFont(hungarian_font_16x16);
}

void saveConfig(int sensor_index){
  int addr = getSensorAddress(sensor_index);
  if ( night_mode ) {
    addr += NIGHT_OFFSET;
  } else {
    addr += ALARM_OFFSET;
  }
  EEPROM.get(addr, alarm);
}

void showAlarmSettingGUI(int sensor_index=0, bool night=false) {
  
  initAlarmSettingsGUI(sensor_index, night);

  if ( night ) {
    showTitle("\\jszakai h#m|rs|klet be{ll\"t{s");
  } else {
    showTitle("H#m|rs|klet be{ll\"t{s");
  }
  but_back = showBackButton();
  showRooms(sensor_index);
  showSettings(alarm, sensor_index);

  Serial.print("ALARM GUI (loop):");
  if (myTouch.dataAvailable() == true) {
    pressed_button = myButtons.checkButtons();
    if (pressed_button==room_button[0]) {
      saveConfig(sensor_index);
      alarm = 0;
      initAlarmSettingsGUI(0);
    } else if (pressed_button==room_button[1]) {
      saveConfig(sensor_index);
      alarm = 0;
      initAlarmSettingsGUI(1);
    }
  }
  Serial.println("ALARM GUI (sleep):");
}


#endif
