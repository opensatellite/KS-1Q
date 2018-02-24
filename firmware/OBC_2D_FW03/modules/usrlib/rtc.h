#ifndef _RTC_H__
#define _RTC_H__

#include <time.h>
//reimplementation time_t time(time_t *timer);

void rtc_init();
void rtc_set(time_t now);

#endif