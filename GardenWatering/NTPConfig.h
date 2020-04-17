#pragma once
#ifndef NTP_CONFIG_H
#define NTP_CONFIG_H

/*int8_t timeZone = 1;
int8_t minutesTimeZone = 0;

const char* ntpServerName = "0.europe.pool.ntp.org";*/
#define TIMEZONE 1
#define MINUTES_TIMEZONE 0

#define NTP_SERVER_NAME "0.europe.pool.ntp.org"
#define NTP_DAYLIGHT_SAVING true

#define DEFAULT_SYNC_PERION 3600

#endif //
