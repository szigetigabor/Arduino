
#define MAIN_PUMP 13    // pin on arduino for main pump

#define OFF true
#define ON false

void setRelay(byte pin, bool value) {
  digitalWrite(pin, value);
  Serial.print("Relay (");
  Serial.print(pin);
  Serial.print(") set to ");
  if ( !value ) {
    Serial.println("ON");
  } else {
    Serial.println("OFF");
  }
}
  



