#include "MCPManagement.h"
#include <Arduino.h>

MCPManagement::MCPManagement(int I2CAddr) {
  mI2CAddr=I2CAddr;
  mcp.begin(mI2CAddr); // use default address 0

  for(int i=0; i<NR_OF_PORTS; i++) {
      //outputs
      mcp.pinMode(i+NR_OF_PORTS, OUTPUT);
      mcp.pullUp(i+NR_OF_PORTS, HIGH);  // turn on a 100K pullup internally
      zoneOutput[i]=LOW;
      setOutput(i, HIGH);  // set OFF all outputs

      // Buttons
      mcp.pinMode(i, INPUT);
      mcp.pullUp(i, HIGH); // turn on a 100K pullup internally
      zoneInput[i]=HIGH;
  }
}

void MCPManagement::buttonPushTriggerCheck() {
  for(int i=0; i<NR_OF_PORTS; i++) {
    oneButtonCheck(i);
  }
}

bool MCPManagement::getInput(int port) {
  if ( port < 0 || port >=NR_OF_PORTS) {
    Serial.println("Wrong port nr is given. Valid values: 0-7");
    return true; // true :: button is not pushed
  }
  return mcp.digitalRead(port);
}

void MCPManagement::setOutput(int port, bool value) {
  if ( port < 0 || port >=NR_OF_PORTS) {
    Serial.println("Wrong port nr is given. Valid values: 0-7");
    return;
  }
  mcp.digitalWrite(port+NR_OF_PORTS, value);
  zoneOutput[port]=value;
}

bool MCPManagement::getOutput(int port) {
  if ( port < 0 || port >=NR_OF_PORTS) {
    Serial.println("Wrong port nr is given. Valid values: 0-7");
    return false;
  }
  return zoneOutput[port];
}

void MCPManagement::oneButtonCheck(int port) {
  if ( port < 0 || port >=NR_OF_PORTS) {
    Serial.println("Wrong port nr is given.");
    return;
  }

  bool currentState = getInput(port);

  if (zoneInput[port] != currentState) {
     zoneInput[port] = currentState;  // store the current state
     Serial.print(mI2CAddr);
     Serial.print(". MCP device: ");
     Serial.print(port+1);
     Serial.print(". Input button state changed: ");
     Serial.println(zoneInput[port] ? "RELEASED" : "PUSHED");
     if ( !currentState ) {
       // button is pushed
       setOutput(port, !zoneOutput[port]);
    }
  }
}
