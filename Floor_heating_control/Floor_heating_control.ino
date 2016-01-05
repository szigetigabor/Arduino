

int ledPin = 13;                 // LED connected to digital pin 13

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output

  //setReset();
  initEEPROM();
    
  Serial.print("End setup function");

}

void loop() {
  // put your main code here, to run repeatedly:

}


