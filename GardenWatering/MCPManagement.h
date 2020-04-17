
#ifndef _McpManagement_h
#define _McpManagement_h

#include "Adafruit_MCP23017.h"

#define NR_OF_PORTS 8

/*
 * MCP23017 management class
 * MCP ports are mapped the following rules:
 *    0-7.  ports: input
 *    8-15. ports: output 
 *    
 * When i. input port is triggered (move to LOW) it triggers the i. output (i+8. MCP) port to change state.
 * 
 */

class MCPManagement {
public:
    /**
    * Construct MCP management.
    */
    MCPManagement(int I2CAddr=0);   //I2C address is between 0 and 7

    void buttonPushTriggerCheck();
    bool getInput(int port);
    bool getOutput(int port);
    void setOutput(int port, bool value);

    String getIdentifier(int port);
    void setIdentifier(int port, String value);
    void setIdentifier(String value[NR_OF_PORTS]);
 
protected:
    void oneButtonCheck(int port);
    void printDebugMessage(int port, bool isInput, String ExtraText="");
//private:
    Adafruit_MCP23017 mcp;
    int  mI2CAddr;
    bool zoneInput[NR_OF_PORTS];
    bool zoneOutput[NR_OF_PORTS];

    String identifier[NR_OF_PORTS];
};

class MCPMomentaryManagement: public MCPManagement  {
public:
    /**
    * Construct MCP management.
    */
    MCPMomentaryManagement(int I2CAddr=0);   //I2C address is between 0 and 7

    void setOutput(int port, bool value);
};

#endif // _McpManagement_h
