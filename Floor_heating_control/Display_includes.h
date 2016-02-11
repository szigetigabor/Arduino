#ifndef DISPLAY_INCLUDES
#define DISPLAY_INCLUDES

#include <UTFT.h>
#include <memorysaver.h>
#include <UTouch.h>
#include <UTFT_Buttons.h>

word DEFAULT_BACKGROUND = VGA_BLACK;
word DEFAULT_FONT_COLOR = VGA_WHITE;
#define DEFAULT_FONT hungarian_font_16x16
#define DEFAULT_BUTTON_SYMBOL Dingbats1_XL

int current_page, prev_page;
bool touched;
int but_back, pressed_button;
unsigned long last_used, _now;
int idle_max = 5000;
bool idle;

#define PAGE_MAIN           1
#define PAGE_TEMPERATURE    2
#define PAGE_SETTINGS       3
#define PAGE_DATE           4
#define PAGE_REQ_TEMP       5
#define PAGE_REQ_NIGHT_TEMP 6
#define PAGE_MODE_TIMING    7
#define PAGE_DISPLAY        8

#define TITLE_HIGH 35
const int displayPWM = 2;

#include <UTFT_Buttons.h>
extern UTFT          myGLCD;
extern UTFT_Buttons  myButtons;
extern UTouch        myTouch;

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
  for (int brightness = 255; brightness >= 0; brightness--) {
    analogWrite(displayPWM, brightness);
    delay(20);
  }
  myGLCD.fillScr(DEFAULT_BACKGROUND);
  myButtons.deleteAllButtons();
}

void displayON() {
  // fade the LED on thisPin from brightnest to off:
  for (int brightness = 0; brightness >= 255; brightness++) {
    analogWrite(displayPWM, brightness);
    delay(20);
  }
}
#endif
