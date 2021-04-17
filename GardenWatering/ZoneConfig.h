//#pragma once
#ifndef ZONE_CONFIG_H
#define ZONE_CONFIG_H

#define NR_OF_ZONES 5

#define DEFAULT_START_HOUR   1
#define DEFAULT_START_MINUTE 0
#define DEFAULT_START_SECOND 1

#define END_SHIFTING_TIME 2

// Time duration in minutes for each zone
#define DEFAULT_ACTIVE_TIME_ZONE 3

#define ACTIVE_TIME_ZONE1 5
#define ACTIVE_TIME_ZONE2 10
#define ACTIVE_TIME_ZONE3 4

//static const int zonePortMap[NR_OF_ZONES] = {D3, D4, D0};

static int ActiveZone = -1;
static bool bZoneButtonMomentary = false;



#endif //ZONE_CONFIG_H
