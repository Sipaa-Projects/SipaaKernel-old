#include "rtc.h"
#include "architecture/ioport.h"

void rtc_write(int reg, int data) {
  outportb(RTC_ADDRESS, reg);
  outportb(RTC_DATA, data);
}

// Read RTC register  
int rtc_read(int reg) {
  outportb(RTC_ADDRESS, reg);
  return inportb(RTC_DATA);
}

rtc_time_t get_current_time()
{
    rtc_time_t time;
    time.second = rtc_read(RTC_SEC);
    time.minute = rtc_read(RTC_MIN);
    time.hour = rtc_read(RTC_HOUR);
    time.day = rtc_read(RTC_DAY);
    time.month = rtc_read(RTC_MONTH);
    time.year = rtc_read(RTC_YEAR);
    time.decade = time.year / 10 + 1;
    time.century = time.year / 100 + 1;
    time.millenial = time.year / 1000 + 1;

    return time;
}

void set_current_time(rtc_time_t time)
{
    rtc_write(RTC_SEC, time.second);
    rtc_write(RTC_MIN, time.minute);
    rtc_write(RTC_HOUR, time.hour);
    rtc_write(RTC_DAY, time.day);
    rtc_write(RTC_MONTH, time.month);
    rtc_write(RTC_YEAR, time.year);
}