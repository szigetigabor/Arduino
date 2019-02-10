//#pragma once
#ifndef ZONE_CONFIG_H
#define ZONE_CONFIG_H

#define NR_OF_ZONES 3

#define DEFAULT_START_HOUR   1
#define DEFAULT_START_MINUTE 0

// Time duration in minutes for each zone
#define DEFAULT_ACTIVE_TIME_ZONE 3

#define ACTIVE_TIME_ZONE1 1
#define ACTIVE_TIME_ZONE2 2
#define ACTIVE_TIME_ZONE3 30

static const int zonePortMap[DEFAULT_ACTIVE_TIME_ZONE] = {D3, D4, D0};



#endif //ZONE_CONFIG_H
