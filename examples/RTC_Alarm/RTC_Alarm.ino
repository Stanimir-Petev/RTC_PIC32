/*
  Simple demo to demonstrate funcionality of RTC alarm feature on PIC32. Tested with:
  IDE: Arduino 1.8.13
  Board: Olimex PIC32-Pinguino-OTG rev.F
  Packages:
  https://raw.githubusercontent.com/OLIMEX/Arduino_configurations/master/PIC/package_olimex_pic_index.json
  https://raw.githubusercontent.com/chipKIT32/chipKIT-core/master/package_chipkit_index.json
  
  Demo description:
  On the terminal will be displayed the date and time every second
  strting from 28th February 2020 (Friday) at 23:59:52
  Upon reaching the set alarm time (in the example 29th February 2020 at 0:0:5)
  on the terminal will be shown message for alarm being triggered
  and the onboard LED (by default D14) will be on.
  To turn it off hold the onboard button (by default D17).
*/

#include <RTC_PIC32.h>

#define ALARM_LED    14
#define ALARM_BUTTON 17

char Text[64];

RTC_PIC32 RTC(20,2,28,5,23,59,52);  // set the initial time at 2020/02/28 (Friday) 23:59:52

void setup()
{
  Serial.begin (115200);
  RTC.Init ();
  RTC.Set_Alarm (2, 29, 6, 0, 0, 5, ALARM_EVERY_DAY, 3); // alarm set to 29 February - 0:0:5, once every day, 3 times

  // LED initialized to indicate alarm trigger
  pinMode (ALARM_LED, OUTPUT);
  digitalWrite (ALARM_LED, LOW);
  pinMode (ALARM_BUTTON, INPUT);
}

void loop()
{
  static int PrevSeconds = 0; // variable used to prevent constant spam of messages but instead once every second
  if (RTC.Get_Second() != PrevSeconds)
  {
    sprintf (Text, "Date: 20%d/%d/%d (%s); Time: %d:%d:%d\n", RTC.Get_Year(), RTC.Get_Month(), RTC.Get_Day(), WeekDay[RTC.Get_DotW()], RTC.Get_Hour(), RTC.Get_Minute(), RTC.Get_Second());
    Serial.print (Text);
    PrevSeconds = RTC.Get_Second();
    
    if (IFS1 & 0x8000)  // check if the Alarm interrupt flag is high
    {
      digitalWrite (ALARM_LED, !digitalRead(ALARM_LED));  // toggle the LED
      Serial.println ("Alarm triggered!");
      if (!digitalRead (ALARM_BUTTON))
      {
        IFS1 = IFS1 & 0x7FFF; // clear the flag
        Serial.println ("Alarm stopped!");
      }
    }
  }
}