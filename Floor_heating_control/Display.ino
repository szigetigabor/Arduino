#include "Display_includes.h"
#include "mainGUI.h"
#include "alarmGUI.h"

int i; //TODO remove

//UTFT          myGLCD(HX8357B,38,39,40,41);
//UTFT          myGLCD(QDM320B,38,39,40,41);
UTFT          myGLCD(CTE32HR,38,39,40,41);


UTouch        myTouch(6,5,4,3,2);

// Finally we set up UTFT_Buttons :)
UTFT_Buttons  myButtons(&myGLCD, &myTouch);


void initDisplay(){
  myGLCD.InitLCD();//PORTRAIT);
  myGLCD.clrScr();
  myGLCD.setFont(hungarian_font_16x16);
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

  //myGLCD.fillScr(255,255,255);
  myGLCD.fillScr(VGA_BLACK);

  myGLCD.setBrightness(1);
  myGLCD.setContrast(1);

  myTouch.InitTouch(PORTRAIT);
  myTouch.setPrecision(PREC_MEDIUM);
  
  myButtons.setTextFont(hungarian_font_16x16);
  myButtons.setSymbolFont(Dingbats1_XL);

  current_page = 1;
}

void showGUI(){
  switch( current_page) {
    case 1:
      showMainGUI();
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      showAlarmSettingGUI(i);
      break;
    case 6:
      showAlarmSettingGUI(i,true);
      break;
    case 7:
      break;
    default:
    break;

  }
  delay(2000);
  i = (i+1) % 4;
  Serial.println(i);
}

