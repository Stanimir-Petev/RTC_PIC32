#ifndef _RTC_PIC32_H_
#define _RTC_PIC32_H_

#define	ALARM_EVERY_HALF_SECOND 0x0
#define	ALARM_EVERY_SECOND      0x1
#define	ALARM_EVERY_10_SECONDS  0x2
#define	ALARM_EVERY_MINUTE      0x3
#define	ALARM_EVERY_10_MINUTE   0x4
#define	ALARM_EVERY_HOUR        0x5
#define	ALARM_EVERY_DAY         0x6
#define	ALARM_EVERY_WEEK        0x7
#define	ALARM_EVERY_MONTH       0x8
#define	ALARM_EVERY_YEAR        0x9

extern const char WeekDay[7][10];

int BCD_to_Dec (int Input);
int Dec_to_BCD (int Input);

class RTC_PIC32
{
  private:
  public:
    RTC_PIC32 ();
    RTC_PIC32 (int Hour, int Minute, int Second);
    RTC_PIC32 (int Year, int Month, int Day, int DotW, int Hour, int Minute, int Second);
    void Init ();
    
    void Set_Time (int Hour, int Minute, int Second);
    void Set_Date (int Year, int Month, int Day);
    void Set_DotW (int DayOfTheWeek);

    int Get_Year ();
    int Get_Month ();
    int Get_Day ();
    int Get_DotW ();
    int Get_Hour ();
    int Get_Minute ();
    int Get_Second ();

    void Set_Alarm (int Month, int Day, int DayOfTheWeek, int Hour, int Minute, int Second, int Mask, int Counter);
    void Set_AlarmTime (int Hour, int Minute, int Second, int Mask, int Counter);
    void Set_AlarmDate (int Month, int Day, int Mask, int Counter);
    void Set_AlarmDotW (int DayOfTheWeek, int Mask, int Counter);

    int Get_AlarmMonth ();
    int Get_AlarmDay ();
    int Get_AlarmDotW ();
    int Get_AlarmHour ();
    int Get_AlarmMinute ();
    int Get_AlarmSecond ();
};

#endif
