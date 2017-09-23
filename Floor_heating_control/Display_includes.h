#ifndef DISPLAY_INCLUDES
#define DISPLAY_INCLUDES

#include <UTFT.h>
#include <memorysaver.h>
#include <URTouch.h>
#include <UTFT_Buttons.h>

word DEFAULT_BACKGROUND = VGA_BLACK;
word DEFAULT_FONT_COLOR = VGA_WHITE;
#define DEFAULT_FONT hungarian_font_16x16
#define DEFAULT_BUTTON_SYMBOL Dingbats1_XL

int current_page, prev_page;
bool touched;
int but_back, pressed_button, but_start;
unsigned long last_used, _now;
unsigned int idle_max;
unsigned int values[]   = { 0, 10000, 20000, 30000, 60000};
bool idle;

#define PAGE_MAIN           1
#define PAGE_TEMPERATURE    2
#define PAGE_SETTINGS       3
#define PAGE_DATE           4
#define PAGE_REQ_TEMP       5
#define PAGE_REQ_NIGHT_TEMP 6
#define PAGE_MODE_TIMING    7
#define PAGE_DISPLAY        8
#define PAGE_CURRENTS       9
#define PAGE_DISPLAY_TEST   10
#define PAGE_SETTINGS2      11
#define PAGE_MODE           12
#define PAGE_RESET          13
#define PAGE_SCT            14

#define TITLE_HIGH 35
#define ONE_MINUTES 60000
const int displayPWM = 7;

#include <UTFT_Buttons.h>
extern UTFT          myGLCD;
extern UTFT_Buttons  myButtons;
extern URTouch       myTouch;

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t Dingbats1_XL[];
extern uint8_t franklingothic_normal[];
//extern uint8_t SevenSegNumFontPlusPlus[];
extern uint8_t GroteskBold16x32[];
extern uint8_t GroteskBold32x64[];
extern uint8_t GroteskBoldOwn32x64[];
extern uint8_t hungarian_font_16x16[];

void showTitle( char* title ) {
  myGLCD.setColor(VGA_RED);
  myGLCD.fillRect(0, 0, 480, TITLE_HIGH);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.drawRect(0, 0, 479, TITLE_HIGH);
  myGLCD.setBackColor(VGA_RED);
  myGLCD.print(title, CENTER, 10);
  myGLCD.setColor(VGA_BLUE);

  //set default color
  myGLCD.setColor(DEFAULT_FONT_COLOR);
  myGLCD.setBackColor(DEFAULT_BACKGROUND);

}

int showBackButton() {
  //back button
  int but_back = myButtons.addButton( 0, 0, 50,  50, "9", BUTTON_SYMBOL);
  myButtons.drawButton(but_back);
  return but_back;  
}


void displayOFF() {
  // fade the LED on thisPin from brightnes to off:
/*  for (int brightness = 255; brightness >= 0; brightness--) {
    analogWrite(displayPWM, brightness);
    delay(20);
  }*/
  myGLCD.fillScr(DEFAULT_BACKGROUND);
  myButtons.deleteAllButtons();
  if (debug >= INFO) {
    Serial.println("Display OFF");
  }
}

void displayON() {
  // fade the LED on thisPin from brightnest to off:
  for (int brightness = 0; brightness >= 255; brightness++) {
    analogWrite(displayPWM, brightness);
    delay(20);
  }
  if (debug >= INFO) {
    Serial.println("Display ON");
  }
}

int show_startButton() {
  int but_id = myButtons.addButton( 0, 0, 200,  50, "Start OK");
  myButtons.drawButton(but_id);
  return but_id;
}

void showTouchPoint(int start_but) {
  int x,y;
  //back button
  bool ok = false;

  while (!ok) {
    _now = millis();
    if ( abs(_now - last_used) > idle_max ) {
      //Serial.println("túl régen indultunk, indít system.");
      current_page = PAGE_DISPLAY_TEST;
      last_used = millis();
      ok = true;                    // only to run normal functionality
    } else {
      if (myTouch.dataAvailable() == true) {
        last_used = millis();
        myTouch.read();
        x = myTouch.getX();
        y = myTouch.getY();
        if (debug >= DEBUG) {
          Serial.print("X: ");Serial.print(x);Serial.print(", ");
          Serial.print("Y: ");Serial.print(y);Serial.println("");
        }
        if ((x!=-1) and (y!=-1)) {
          myGLCD.drawPixel(x,y);
        }
        pressed_button = myButtons.checkButtons();
        if (pressed_button==start_but) {
          if (debug >= INFO) {
            Serial.println("Touch display: OK!");
          }
          current_page = PAGE_MAIN;
          ok = true;
        }
      }
    }
  }
}
#endif
