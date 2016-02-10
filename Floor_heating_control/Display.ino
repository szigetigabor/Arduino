#include "Display_includes.h"
#include "mainGUI.h"
#include "dateTimeGUI.h"
#include "alarmGUI.h"
#include "settingsGUI.h"
#include "temperatureGUI.h"

int i; //TODO remove

/* Update UTouchCD.h for a right touch feeling.
 * values for my 3.5" TFTLCD (ILI9481) for arduino 2560 from mcufriend.com
 * #define CAL_X 0x00178F66UL
 * #define CAL_Y 0x03B440FCUL
 * #define CAL_S 0x801DF13FUL
 *  
 */

//UTFT          myGLCD(HX8357B,38,39,40,41);
//UTFT          myGLCD(QDM320B,38,39,40,41);
//UTFT          myGLCD(CTE32HR,38,39,40,41);
UTFT          myGLCD(ILI9481,38,39,40,41);


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

  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_EXTREME);
  
  myButtons.setTextFont(DEFAULT_FONT);
  myButtons.setSymbolFont(DEFAULT_BUTTON_SYMBOL);

  current_page = 5;
  prev_page = 0;
  touched = false;
}

void showGUI(){
  switch( current_page) {
    case PAGE_MAIN:
      showMainGUI();
      break;
    case PAGE_TEMPERATURE:
      showTemperatures();
      break;
    case PAGE_SETTINGS:
      showSettingsGUI();
      break;
    case PAGE_DATE:
      showDateTimeSettingGUI();
      break;
    case PAGE_REQ_TEMP:
      showAlarmSettingGUI();
      break;
    case PAGE_REQ_NIGHT_TEMP:
      showAlarmSettingGUI(true);
      break;
    case PAGE_MODE_TIMING:
      //showScheduleSettingGUI();
      break;
    case PAGE_DISPLAY:
      //showDisplaySettingGUI();
      break;
    default:
    break;

  }
  //delay(2000);
 // i = (i+1) % 4;
  //Serial.println(i);
}

