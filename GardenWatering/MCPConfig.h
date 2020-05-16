#pragma once
#ifndef MCP_CONFIG_H
#define MCP_CONFIG_H

#define MCP_MOMENTARY_ID  0
#define MCP_BUTTONS_ID    1

extern String momentaryLabels[NR_OF_PORTS];
extern String buttonLabels[NR_OF_PORTS];


/*
 * Singleton MCP objects
 */
static MCPManagement* getButtonPtr() {
  static MCPManagement* pButton;
  if ( pButton == nullptr) {
    pButton = new MCPManagement(MCP_BUTTONS_ID);
    Serial.print("new Button address (");
    Serial.print((unsigned int)pButton, HEX);
    Serial.println(")");
  }
  return pButton;
}

static MCPMomentaryManagement* getMomentaryButtonPtr() {
  static MCPMomentaryManagement* pMomentary;
  if ( pMomentary == nullptr) {
    pMomentary = new MCPMomentaryManagement(MCP_MOMENTARY_ID);
    Serial.print("new Momentary address (");
    Serial.print((unsigned int)pMomentary, HEX);
    Serial.println(")");
  }
  return pMomentary;
}
#endif //MCP_CONFIG_H
