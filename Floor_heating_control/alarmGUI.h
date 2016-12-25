#ifndef ALARM_GUI
#define ALARM_GUI

#include "EEPROM_control.h"
#include "Display_includes.h"

extern float last_temp[];

int room_button[ROOMS];
int but_up, but_down;
int current_sensor;
float alarm;
bool  night_mode;

void initAlarmSettingsGUI(int sensor_index=0, bool night=false) {
  myButtons.deleteAllButtons();
  alarm=0;
  current_sensor = sensor_index;
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
  //Serial.print("ALARM GUI (init):");
  if ( night ) {
    prev_page=PAGE_REQ_NIGHT_TEMP;
  } else {
    prev_page=PAGE_REQ_TEMP;
  }
  
  myGLCD.clrScr();
}

void showRooms(int used) {
  int x = 10;
  int start = 55;
  TempSensorData value;
  //Serial.println("ROOMS: ");
  for(int i=0; i<ROOMS; i++) {
    int addr = getSensorAddress(i);
    
    EEPROM.get(addr, value); 
    if ( i == used ) {
      myButtons.setButtonColors(VGA_WHITE, VGA_RED, VGA_WHITE, VGA_RED, VGA_BLUE);
      room_button[i] = myButtons.addButton( x,  start, 180,  45, value.name, BUTTON_DISABLED);
    } else {
      myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_WHITE, VGA_RED, VGA_BLUE);
      room_button[i] = myButtons.addButton( x,  start, 180,  45, value.name);
    }
    //Serial.print(",");
    //Serial.print(room_button[i]);
    myButtons.drawButton(room_button[i]);
    start += 55;
  }
}

void showAlarm(float alarm) {
  int x = 220;
  int y = 55;
  // show alarm value
  myGLCD.setFont(GroteskBold32x64);
  x += 46;
  y += 120;
  myGLCD.printNumF(alarm, 1, x, y);
  myGLCD.setFont(franklingothic_normal);
  myGLCD.print("O", x+128, y);
  myGLCD.setFont(GroteskBold32x64);
  myGLCD.print("C", x+128+16, y);
  myGLCD.setFont(hungarian_font_16x16);  
}

void showSettings(float alarm, int index) {
  int x = 220;
  int y = 55;
  myGLCD.print("Akt.h#m.:", x, y);
  myGLCD.printNumF(last_temp[index], 2, x+140, y);
  myGLCD.print("%C", x+220, y);

  // up and down buttons
  but_up   = myButtons.addButton( x,  y+35, 250,  60, "up");
  but_down = myButtons.addButton( x,  y+200, 250,  60, "down");
  myButtons.drawButton(but_up);
  myButtons.drawButton(but_down);

  showAlarm(alarm);
}

void saveConfig(int sensor_index){
  int addr = getSensorAddress(sensor_index);
  if ( night_mode ) {
    addr += NIGHT_OFFSET;
  } else {
    addr += ALARM_OFFSET;
  }
  EEPROM.put(addr, alarm);
}

void showAlarmSettingGUI(bool night=false) {
  if ( current_page != prev_page || touched) {
    initAlarmSettingsGUI(current_sensor, night);
    touched = false;
  

    if ( night ) {
      showTitle("\\jszakai h#m|rs|klet be{ll\"t{s");
    } else {
      showTitle("H#m|rs|klet be{ll\"t{s");
    }
    but_back = showBackButton();
    showSettings(alarm, current_sensor);
    showRooms(current_sensor);
  }

  if (myTouch.dataAvailable() == true) {
    last_used = millis();
    pressed_button = myButtons.checkButtons();
    if (pressed_button==room_button[0]) {
      if ( debug >= DEBUG ) {
        Serial.println("room 1");
      }
      saveConfig(current_sensor);
      alarm = 0;
      touched = true;
      initAlarmSettingsGUI(0);
    } else if (pressed_button==room_button[1]) {
      if ( debug >= DEBUG ) {
        Serial.println("room 2:");
      }
      saveConfig(current_sensor);
      alarm = 0;
      touched = true;
      initAlarmSettingsGUI(1);
    } else if (pressed_button==room_button[2]) {
      if ( debug >= DEBUG ) {
        Serial.println("room 3:");
      }
      saveConfig(current_sensor);
      alarm = 0;
      touched = true;
      initAlarmSettingsGUI(2);
    } else if (pressed_button==room_button[3]) {
      if ( debug >= DEBUG ) {
        Serial.println("room 4:");
      }
      saveConfig(current_sensor);
      alarm = 0;
      touched = true;
      initAlarmSettingsGUI(3);
    } else if (pressed_button==room_button[4]) {
      if ( debug >= DEBUG ) {
        Serial.println("room 5:");
      }
      saveConfig(current_sensor);
      alarm = 0;
      touched = true;
      initAlarmSettingsGUI(4);
    } else if (pressed_button==but_up) {
      if ( debug >= DEBUG ) {
        Serial.println("up");
      }
      alarm += 0.1;
      showAlarm(alarm);
    } else if (pressed_button==but_down) {
      if ( debug >= DEBUG ) {
        Serial.println("down");
      }
      alarm -= 0.1;
      showAlarm(alarm);
    } else if (pressed_button==but_back) {
      if ( debug >= DEBUG ) {
        Serial.println("BACK");
      }
      saveConfig(current_sensor);
      current_page = PAGE_SETTINGS;
    }
  }
}


#endif
