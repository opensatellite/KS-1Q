#include "rtc.h"
#include "os.h"

static time_t rtc_counter;

void rtc_increment(void)
{
    static int divider = 0;
    divider++;
    if(divider >= SYSTICK_FREQ)
    {
        divider = 0;
        rtc_counter++;
    }
}

void rtc_init(void)
{
    //make time from build number
    struct tm t;
    t.tm_sec = 0;
    t.tm_min = 0;
    t.tm_hour = 0;
    t.tm_mday = 1;
    t.tm_mon = 1;
    t.tm_year = 116;
    //start from 2000
    t.tm_isdst = false;
    rtc_counter = mktime(&t);
    
    systick_attach(rtc_increment);
}

void rtc_set(time_t now)
{
    rtc_counter = now;
}

#if _DLIB_TIME_USES_64
time_t __time64 (time_t* timer)
#else
time_t __time32 (time_t* timer)
#endif
{
    if(timer)
        *timer = rtc_counter;
    return rtc_counter;
}
