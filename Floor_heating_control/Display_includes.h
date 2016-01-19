#ifndef DISPLAY_INCLUDES
#define DISPLAY_INCLUDES

#include <UTFT.h>
#include <memorysaver.h>
#include <UTouch.h>
#include <UTFT_Buttons.h>

word DEFAULT_BACKGROUND = VGA_BLACK;
word DEFAULT_FONT_COLOR = VGA_WHITE;

int current_page;

#define PAGE_MAIN           1
#define PAGE_TEMPERATURE    2
#define PAGE_SETTINGS       3
#define PAGE_DATE           4
#define PAGE_REQ_TEMP       5
#define PAGE_REQ_NIGHT_TEMP 6
#define PAGE_MODE_TIMING    7
#define PAGE_DISPLAY        8

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
extern uint8_t GroteskBold32x64[];
extern uint8_t hungarian_font_16x16[];

void showTitle( char title[20], int offset ) {
  myGLCD.setColor(VGA_BLUE);
  myGLCD.fillRect(0, 0, 480, 35);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.setBackColor(VGA_BLUE);
  myGLCD.print(title, 240-offset, 10);

  //set default color
  myGLCD.setColor(DEFAULT_FONT_COLOR);
  myGLCD.setBackColor(DEFAULT_BACKGROUND);

}

int showBackButton() {
  //back button
  int but_back = myButtons.addButton( 1, 1, 50,  50, "9", BUTTON_SYMBOL);
  myButtons.drawButton(but_back);
  return but_back;  
}


#endif
