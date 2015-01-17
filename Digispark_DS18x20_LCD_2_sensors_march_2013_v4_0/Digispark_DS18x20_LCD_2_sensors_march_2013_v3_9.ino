/* 
 April 01, 2013
 Digispark Arduino interface 1.03 used to compile
 Digispark(Tiny Core)
 ATtiny85 Clock PLL-16Mhz
 LCD control PCF8475A ([A]extended address chip) wired to use the DFRRobot Library
 Eagle board "LCD 8x2 dual DS18x20 Ver2.brd"
 (actually uses a header for 16x2 LCD)
 
 I2C LCD connects to SDA, SCL +5v, Gnd 
 Uses a 16x2 I2C LCD Backlight Display  
 
 DS18B20x2 Temperature chip on pin P4
 4k7 pullup between pin P4 and +5v.
 Hard code addresses of sensor to make code smaller and easier.     
 
 Pin connections of ATtiny85
 P0 = SDA      Pin 5
 P1 =          Pin 6
 P2 = SCL      Pin 7
 P3 =          Pin 2
 P4 = DS18B20  Pin 3
 P5 =          Pin 1
 
 */

#include <OneWire.h>
#define ONE_WIRE_BUS 4                  // DS18B20 is on P4
#include <TinyWireM.h>                  // I2C Master lib for ATTinys which use USI - comment this out to use with standard arduinos
#include <LiquidCrystal_I2C.h>          // for LCD w/ GPIO MODIFIED for the ATtiny85
#define GPIO_ADDR     0x27    //LCD address, PCF8475A extended address  

OneWire ds(ONE_WIRE_BUS);
LiquidCrystal_I2C lcd(GPIO_ADDR,16,2);  // set address & 16 chars / 2 lines

int led_inside = 3;
int T_inside = 22000;
boolean ON_inside = false;

int led_outside = 1;
int T_outside= 21300;
boolean ON_outside = false;

//DS18S20 temperature senor MAC address
//AF00043EC07EFF28
byte addr[8]={0x28, 0xFF, 0x7E, 0xC0, 0x3E, 0x04, 0x00, 0xAF};//long sensor cable [outside]

//DS18S20 temperature senor MAC address
//FA00043D89A8FF28
byte addr1[8]={0x28, 0xFF, 0xA8, 0x89, 0x3D, 0x04, 0x00, 0xFA};//Short sensor cable [inside]

//
int HighByte, LowByte, TReading, SignBit, Tc_100, Whole, Fract;
char buf[20];

int HighBytea, LowBytea, TReadinga, SignBita, Tc_100a, Wholea, Fracta;
char bufa[30];

//________________________________________________________________________________  
void setup(void) {
  // set up the LCD's number of rows and columns: 

  TinyWireM.begin();               // initialize I2C lib

  lcd.init();                      // initialize the lcd 
  lcd.clear();
  lcd.noCursor();
  lcd.backlight();                 // Turn on the backlight

    // Print a message to the LCD.
  lcd.setCursor(0,0);                 // First position (0) and first line (0) 
  lcd.print ("Arduino ");
  lcd.setCursor(2,1);                 // First position (0) and second line (1)
  lcd.print ("Temp Sensors");
  delay(4000); 
  lcd.clear();

  // initialize the digital pin as an output.
  pinMode(led_inside, OUTPUT);     
  pinMode(led_outside, OUTPUT);     

}
//________________________________________________________________________________

void loop(void) {
  //sensorRead();
  sensor2Read();
  delay(3000);

}

//________________________________________________________________________________  

void sensorRead()
{ 
  byte i;
  byte present = 0;
  byte data[15];

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 12; i++) 
  {           // we need 9 bytes
    data[i] = ds.read();
  }

  LowByte = data[0];
  HighByte = data[1];
  TReading = (HighByte << 8) + LowByte;
  SignBit = TReading & 0x8000;  // test most sig bit
  if (SignBit) // negative
  {
    TReading = (TReading ^ 0xffff) + 1; // 2's comp
  }
  Tc_100 = (TReading*100/2);    

  Whole = Tc_100 / 1000;  // separate off the whole and fractional portions
  Fract = Tc_100 % 1000;

  if ( Tc_100 >= T_inside ) {
    if ( ON_inside == false ) {
      digitalWrite(led_inside, HIGH);
      ON_inside = true;
    }
  } 
  else {
    digitalWrite(led_inside, LOW);
    ON_inside = false;
  }

  sprintf(buf, "%c%d.%d\337C     ",SignBit ? '-' : '+', Whole, Fract < 10 ? 0 : Fract/10);
  lcd.setCursor(0,0);
  lcd.print(buf);
  lcd.setCursor(9,0);
  lcd.print("Outside");
}


//________________________________________________________________________________ 


void sensor2Read()
{
  byte j;
  byte presenta = 0;
  byte dataa[12];

  ds.reset();
  ds.select(addr1);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  presenta = ds.reset();
  ds.select(addr1);    
  ds.write(0xBE);         // Read Scratchpad

  for ( j = 0; j < 9; j++) 
  {           // we need 9 bytes
    dataa[j] = ds.read();
  }

//start
  int16_t raw = (dataa[1] << 8) | dataa[0];
    byte cfg = (dataa[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  float celsius = (float)raw / 16.0;
  int dec = (celsius - (int)celsius)*100;
  
  lcd.setCursor(0,0);
    //lcd.print(celsius);
  SignBita=0;
  if (celsius < 0.0) {
    SignBita=1;
  }
  sprintf(bufa, "%c%d.%d\337C     ",SignBita ? '-' : '+',(int)celsius, dec);
  //lcd.setCursor(0,1);
  lcd.print(bufa);
  
 //   lcd.setCursor(1,1);
   // lcd.print(celsius);
//end




  LowBytea = dataa[0];
  HighBytea = dataa[1];
  TReadinga = (HighBytea << 8) + LowBytea;
  SignBita = TReadinga & 0x8000;  // test most sig bit
  if (SignBita) // negative
  {
    TReadinga = (TReadinga ^ 0xffff) + 1; // 2's comp
  }
  Tc_100a = (TReadinga*100/2);    
/*

  if ( Tc_100a >= T_outside ) {
    if ( ON_outside == false ) {
      digitalWrite(led_outside, HIGH);
      ON_outside = true;
    }
  } 
  else {
    digitalWrite(led_outside, LOW);
    ON_outside = false;
  }

  Wholea = Tc_100a / 1000;  // separate off the whole and fractional portions
  Fracta = Tc_100a % 1000;

  sprintf(bufa, "%c%d.%d\337C     ",SignBita ? '-' : '+', Wholea, Fracta < 10 ? 0 : Fracta/10);
  lcd.setCursor(0,1);
  lcd.print(bufa);
  lcd.setCursor(9,1);
  lcd.print("Inside");*/

}



