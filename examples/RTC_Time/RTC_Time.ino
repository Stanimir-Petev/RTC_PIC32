/*
  Simple demo to demonstrate funcionality of RTC on PIC32. Tested with:
  IDE: Arduino 1.8.13
  Board: Olimex PIC32-Pinguino-OTG rev.F
  Packages:
  https://raw.githubusercontent.com/OLIMEX/Arduino_configurations/master/PIC/package_olimex_pic_index.json
  https://raw.githubusercontent.com/chipKIT32/chipKIT-core/master/package_chipkit_index.json
  
  Demo description:
  On the terminal will be displayed the date and time every second
  strting from 28th February 2020 (Friday) at 23:59:52
*/

#include <RTC_PIC32.h>

char Text[64];

RTC_PIC32 RTC(20,2,28,5,23,59,52);  // set the initial time at 2020/02/28 (Friday) 23:59:52

void setup()
{
  Serial.begin (115200);
  RTC.Init ();
}

void loop()
{
  static int PrevSeconds = 0; // variable used to prevent constant spam of messages but instead once every second
  if (RTC.Get_Second() != PrevSeconds)
  {
    sprintf (Text, "Date: 20%d/%d/%d (%s); Time: %d:%d:%d\n", RTC.Get_Year(), RTC.Get_Month(), RTC.Get_Day(), WeekDay[RTC.Get_DotW()], RTC.Get_Hour(), RTC.Get_Minute(), RTC.Get_Second());
    Serial.print (Text);
    PrevSeconds = RTC.Get_Second();
  }
}