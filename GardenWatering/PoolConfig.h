//#pragma once
#ifndef POOL_CONFIG_H
#define POOL_CONFIG_H

#define POOL_PORT 8

#define FIX_SCHEDULING 1

#ifdef FIX_SCHEDULING
    #define NR_OF_POOL_ALARMS 6    // fix nr
#else
    #define NR_OF_POOL_ALARMS 16    // 24h / DEFAULT_POOL_TRIGGER_FREQUENCY * 2
#endif

#define DEFAULT_POOL_START_HOUR   18
#define DEFAULT_POOL_START_MINUTE 0

// Frequency of pool triggers in hour
#define DEFAULT_POOL_TRIGGER_FREQUENCY 3

// Active pool time duration in minutes for each period
#define DEFAULT_ACTIVE_TIME_POOL 60

static bool bPoolActive = false;
static bool bPoolButtonMomentary = false;



#endif //POOL_CONFIG_H
