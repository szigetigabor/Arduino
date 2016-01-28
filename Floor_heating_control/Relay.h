
void setRelay(byte pin, bool value) {
  digitalWrite(pin, value);
  Serial.print("Relay (");
  Serial.print(pin);
  Serial.print(") set to ");
  if ( value ) {
    Serial.println("ON");
  } else {
    Serial.println("OFF");
  }
}
  


