//#pragma once
#ifndef MCP_CONFIG_H
#define MCP_CONFIG_H

#define MCP_MOMENTARY_ID  1 //TODO set back to 0
#define MCP_BUTTONS_ID    1

static MCPMomentaryManagement momentary(MCP_MOMENTARY_ID);
static MCPManagement buttons(MCP_BUTTONS_ID);

extern String momentaryLabels[NR_OF_PORTS];
extern String buttonLabels[NR_OF_PORTS];


#endif //MCP_CONFIG_H
