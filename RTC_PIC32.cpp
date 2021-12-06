#include "Arduino.h"
#include "RTC_PIC32.h"

const char WeekDay[7][10] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int BCD_to_Dec (int Input)
{
  int Number = 0, Power=1;
  while (Input)
  {
    Number = Number + (Input & 0xF)*Power;
    Input = Input>>4;
    Power = Power * 10;
  }
  return Number;
}

int Dec_to_BCD (int Input)
{
  int Number = 0, Power = 1;
  while (Input)
  {
    Number = Number + (Input%10)*Power;
    Input = Input / 10;
    Power = Power * 16;
  }
  return Number;
}

RTC_PIC32::RTC_PIC32 ()
{
  RTCCON = 0x8;		// RTC Write enable set to 1 (bit 3) so any RTC registers can be written
  RTCTIME = 0;
}

RTC_PIC32::RTC_PIC32 (int Hour, int Minute, int Second)
{
  RTCCON = 0x8;		// RTC Write enable set to 1 (bit 3) so any RTC registers can be written
  Set_Time (Hour, Minute, Second);
}

RTC_PIC32::RTC_PIC32 (int Year, int Month, int Day, int DotW, int Hour, int Minute, int Second)
{
  RTCCON = 0x8;		// RTC Write enable set to 1 (bit 3) so any RTC registers can be written
  Set_Date (Year, Month, Day);
  Set_DotW (DotW);
  Set_Time (Hour, Minute, Second);
}

void RTC_PIC32::Init ()
{
  OSCCON = OSCCON | 0x02;	// enable secondary oscillator --> SOSCEN (bit 1) = 1
  while (!(OSCCON & 0x400000));	// wait until secondary oscillator is ready (bit 22 should be 1)
  
  RTCCON = 0x8;				// RTC Write enable set to 1 (bit 3) so any RTC registers can be written
  RTCCON = RTCCON | 0x8040;	// RTC module enable (bit 15) and clock is actively running (bit 6)
}

void RTC_PIC32::Set_Date (int Year, int Month, int Day)
{
  RTCDATE = (RTCDATE & 0xF) | (Dec_to_BCD(Year)<<24) | (Dec_to_BCD(Month)<<16) | (Dec_to_BCD(Day)<<8);
}

void RTC_PIC32::Set_DotW (int DayOfTheWeek)
{
  RTCDATE = (RTCDATE & 0xFFFFFF00) | DayOfTheWeek;
}

void RTC_PIC32::Set_Time (int Hour, int Minute, int Second)
{
  RTCTIME = (Dec_to_BCD(Hour)<<24) | (Dec_to_BCD(Minute)<<16) | (Dec_to_BCD(Second)<<8);
}

int RTC_PIC32::Get_Year ()
{
  return BCD_to_Dec ((RTCDATE>>24)&0xFF);
}

int RTC_PIC32::Get_Month ()
{
  return BCD_to_Dec ((RTCDATE>>16)&0xFF);
}

int RTC_PIC32::Get_Day ()
{
  return BCD_to_Dec ((RTCDATE>>8)&0xFF);
}

int RTC_PIC32::Get_DotW ()
{
  return BCD_to_Dec (RTCDATE&0xFF);
}

int RTC_PIC32::Get_Hour ()
{
  return BCD_to_Dec ((RTCTIME>>24)&0xFF);
}

int RTC_PIC32::Get_Minute ()
{
  return BCD_to_Dec ((RTCTIME>>16)&0xFF);
}

int RTC_PIC32::Get_Second ()
{
  return BCD_to_Dec ((RTCTIME>>8)&0xFF);
}

void RTC_PIC32::Set_Alarm (int Month, int Day, int DayOfTheWeek, int Hour, int Minute, int Second, int Mask, int Counter)
{
  while (RTCALRM&0x1000);	// wait for ALRMSYN bit to become 0
  RTCALRMCLR = 0xCFFF;
  ALRMDATE = (Dec_to_BCD(Month)<<16) | (Dec_to_BCD(Day)<<8) | DayOfTheWeek;
  ALRMTIME = (Dec_to_BCD(Hour)<<24) | (Dec_to_BCD(Minute)<<16) | (Dec_to_BCD(Second)<<8);
  RTCALRMSET = 0x8000 | (Mask<<8) | (Counter-1);
}

void RTC_PIC32::Set_AlarmDate (int Month, int Day, int Mask, int Counter)
{
  while (RTCALRM&0x1000);	// wait for ALRMSYN bit to become 0
  RTCALRMCLR = 0xCFFF;
  ALRMDATE = (ALRMDATE & 0xF) | (Dec_to_BCD(Month)<<16) | (Dec_to_BCD(Day)<<8);
  RTCALRMSET = 0x8000 | (Mask<<8) | (Counter-1);
}

void RTC_PIC32::Set_AlarmDotW (int DayOfTheWeek, int Mask, int Counter)
{
  while (RTCALRM&0x1000);	// wait for ALRMSYN bit to become 0
  RTCALRMCLR = 0xCFFF;
  ALRMDATE = (ALRMDATE & 0xFFFFFF00) | DayOfTheWeek;
  RTCALRMSET = 0x8000 | (Mask<<8) | (Counter-1);
}

void RTC_PIC32::Set_AlarmTime (int Hour, int Minute, int Second, int Mask, int Counter)
{
  while (RTCALRM&0x1000);	// wait for ALRMSYN bit to become 0
  RTCALRMCLR = 0xCFFF;
  ALRMTIME = (Dec_to_BCD(Hour)<<24) | (Dec_to_BCD(Minute)<<16) | (Dec_to_BCD(Second)<<8);
  RTCALRMSET = 0x8000 | (Mask<<8) | (Counter-1);
}

int RTC_PIC32::Get_AlarmMonth ()
{
  return BCD_to_Dec ((ALRMDATE>>16)&0xFF);
}

int RTC_PIC32::Get_AlarmDay ()
{
  return BCD_to_Dec ((ALRMDATE>>8)&0xFF);
}

int RTC_PIC32::Get_AlarmDotW ()
{
  return BCD_to_Dec (ALRMDATE&0xFF);
}

int RTC_PIC32::Get_AlarmHour ()
{
  return BCD_to_Dec ((ALRMTIME>>24)&0xFF);
}

int RTC_PIC32::Get_AlarmMinute ()
{
  return BCD_to_Dec ((ALRMTIME>>16)&0xFF);
}

int RTC_PIC32::Get_AlarmSecond ()
{
  return BCD_to_Dec ((ALRMTIME>>8)&0xFF);
}
