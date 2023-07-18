// RTC registers ports
#define RTC_ADDRESS 0x70 
#define RTC_DATA    0x71

// RTC register addresses
#define RTC_SEC     0x00
#define RTC_MIN     0x02
#define RTC_HOUR    0x04
#define RTC_DAY     0x07
#define RTC_MONTH   0x08
#define RTC_YEAR    0x09

typedef struct 
{
    int second;
    int minute;
    int hour;
    int day;
    int month;
    int year;
    int decade;
    int century;
    int millenial;
} rtc_time_t ;

// Write RTC register

rtc_time_t get_current_time();
void set_current_time(rtc_time_t time);