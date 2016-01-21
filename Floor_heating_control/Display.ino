#include "Display_includes.h"
#include "mainGUI.h"
#include "dateTimeGUI.h"
#include "alarmGUI.h"
#include "settingsGUI.h"
#include "temperatureGUI.h"

int i; //TODO remove

//UTFT          myGLCD(HX8357B,38,39,40,41);
//UTFT          myGLCD(QDM320B,38,39,40,41);
UTFT          myGLCD(CTE32HR,38,39,40,41);


UTouch        myTouch(6,5,4,3,2);

// Finally we set up UTFT_Buttons :)
UTFT_Buttons  myButtons(&myGLCD, &myTouch);


void initDisplay(){
  pinMode(displayPWM, OUTPUT);
  
  myGLCD.InitLCD();//PORTRAIT);
  myGLCD.clrScr();
  myGLCD.setFont(DEFAULT_FONT);
  myGLCD.setBackColor(DEFAULT_BACKGROUND);
  //myGLCD.setBackColor(VGA_WHITE);
 // myGLCD.setBackColor(VGA_RED);
  //myGLCD.setBackColor(VGA_GREEN);
  //myGLCD.setBackColor(VGA_BLUE);
  //myGLCD.setBackColor(VGA_SILVER);
  //myGLCD.setBackColor(VGA_GRAY);
  //myGLCD.setBackColor(VGA_MAROON);
  //myGLCD.setBackColor(VGA_YELLOW);
  //myGLCD.setBackColor(VGA_OLIVE);
  //myGLCD.setBackColor(VGA_LIME);
  //myGLCD.setBackColor(VGA_AQUA);
  //myGLCD.setBackColor(VGA_TEAL);
  //myGLCD.setBackColor(VGA_NAVY);
  //myGLCD.setBackColor(VGA_FUCHSIA);
  //myGLCD.setBackColor(VGA_PURPLE);
  //myGLCD.setBackColor(VGA_TRANSPARENT);

  myGLCD.fillScr(DEFAULT_BACKGROUND);

  displayON();

  myTouch.InitTouch(PORTRAIT);
  myTouch.setPrecision(PREC_MEDIUM);
  
  myButtons.setTextFont(DEFAULT_FONT);
  myButtons.setSymbolFont(DEFAULT_BUTTON_SYMBOL);

  current_page = 2;
}

void showGUI(){
  switch( current_page) {
    case 1:
      showMainGUI();
      break;
    case 2:
      showTemperatures();
      break;
    case 3:
      showSettingsGUI();
      break;
    case 4:
      showDateTimeSettingGUI();
      break;
    case 5:
      showAlarmSettingGUI(i);
      break;
    case 6:
      showAlarmSettingGUI(i,true);
      break;
    case 7:
      //showScheduleSettingGUI();
      break;
    case 8:
      //showDisplaySettingGUI();
      break;
    default:
    break;

  }
  delay(20000);
 // i = (i+1) % 4;
  //Serial.println(i);
}

