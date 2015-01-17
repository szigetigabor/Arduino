/* 
 January 13, 2015
 Digispark Arduino interface 1.03 used to compile
 Digispark(Tiny Core)
 ATtiny85 Clock PLL-16Mhz
 
 
 
 DS18B20x2 Temperature chip on pin P2
 4k7 pullup between pin P2 and +5v.
 Hard code addresses of sensor to make code smaller and easier.     
 
 Pin connections of ATtiny85
 P0 = SDA      Pin 5
 P1 = LED      Pin 6
 P2 = DS18B20/SCL      Pin 7
 P3 =  FEAT    Pin 2
 P4 =  SEAT    Pin 3
 P5 =          Pin 1
 
 */

#include <OneWire.h>
#define ONE_WIRE_BUS 2                  // DS18B20 is on P2

OneWire ds(ONE_WIRE_BUS);

int pin_led  = 1;
int pin_seat = 4;
float T_weather_on = 14;     // seat heating turn on
float T_seat_max   = 65;     // seat heating turn off
boolean ON_seat = false;

int pin_feat = 3;
float T_feat_on = 10;        // feat heating turn on
boolean ON_feat = false;


//DS18S20 temperature senor MAC address
//28.FF7C893D0400
//BB00043D897CFF28
byte addr[8]={0x28, 0xFF, 0x7C, 0x89, 0x3D, 0x04, 0x00, 0xBB};//long sensor cable [outside]

//DS18S20 temperature senor MAC address
//28.C0108F040000
//200000048F10C028
byte addr1[8]={0x28, 0xC0, 0x10, 0x8F, 0x04, 0x00, 0x00, 0x20};//Short sensor cable [seat]


//________________________________________________________________________________  
void setup(void) {
  // initialize the digital pin as an output.
  pinMode(pin_led,  OUTPUT);
  pinMode(pin_feat, OUTPUT);     
  pinMode(pin_seat, OUTPUT);    
 
  digitalWrite(pin_seat, LOW);
  digitalWrite(pin_feat, LOW); 
  
  digitalWrite(pin_led, HIGH);
  delay(1000);
  digitalWrite(pin_led, LOW);
  
  delay(60000);

}
//________________________________________________________________________________

void loop(void) {
  float temp_out = sensorRead(addr);
  float temp_circuit = sensorRead(addr1);
  
  // switch the switching
  if ( temp_circuit < T_seat_max ) {
    // seat heating settings
    if ( temp_out < T_weather_on ) {
      if ( ON_seat == false ) {
        digitalWrite(pin_seat, HIGH);
        digitalWrite(pin_led,  HIGH);
        ON_seat = true;
      }   
    } else {
      digitalWrite(pin_seat, LOW);
      digitalWrite(pin_led,  LOW);
      ON_seat = false;
    }
    
    // feat heating settings
    if ( temp_out < T_feat_on ) {
      if ( ON_feat == false ) {
        digitalWrite(pin_feat, HIGH);
        ON_feat = true;
      }   
    } else {
      digitalWrite(pin_feat, LOW);
      ON_feat = false;
    }

  } else {
    digitalWrite(pin_led,  LOW);
    digitalWrite(pin_seat, LOW);
    digitalWrite(pin_feat, LOW);
    ON_seat = false;
    ON_feat = false;   
  }
  
  delay(3000);

}
//________________________________________________________________________________ 


float sensorRead(byte* _addr)
{
  byte j;
  byte present = 0;
  byte data[12];

  ds.reset();
  ds.select(_addr);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(_addr);    
  ds.write(0xBE);         // Read Scratchpad

  // Read and convert temperature
  for ( j = 0; j < 9; j++) 
  {           // we need 9 bytes
    data[j] = ds.read();
  }

  int16_t raw = (data[1] << 8) | data[0];
  byte cfg = (data[4] & 0x60);
  // at lower res, the low bits are undefined, so let's zero them
  if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
  else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
  else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
  //// default is 12 bit resolution, 750 ms conversion time
  float celsius = (float)raw / 16.0;
  //int dec = (celsius - (int)celsius)*100;

  return celsius;
} 





