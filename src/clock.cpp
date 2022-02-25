#include <Wire.h>
#include "clock.h"
#include "config.h" 
#include "server.h"

DateTime _now; 
RTC_DS3231 rtc;

void SetupClock(){
    if (! rtc.begin()) {
        #ifndef RELEASE
            Serial.println("Couldn't find RTC");
            Serial.flush();
        #endif
        while (1) delay(10);
    }

    #ifndef RELEASE
        Serial.print("Datetime in clock setup: ");
    #endif
    displayTime();
}

void GetDateTime(){
    if(rtc.now().day() > 31 || rtc.now().hour() > 24 || rtc.now().minute() > 59 || rtc.now().second() > 59){
        timeClient.update();
        SyncTime();
        #ifndef RELEASE
            Serial.println("Time error! RTC resync");
        #endif
    }
    else {
        _now = rtc.now();
    }
}

float GetTemperature(){
    return rtc.getTemperature();
}

void SyncTime(){
    time_t rawtime = timeClient.getEpochTime();
    struct tm * ti;
    ti = localtime (&rawtime);

    uint8_t date = (ti->tm_mday) < 10 ? 0 + (ti->tm_mday) : (ti->tm_mday);
    uint16_t year = ti->tm_year + 1900 - 2000;
    uint8_t month = (ti->tm_mon + 1) < 10 ? 0 + (ti->tm_mon + 1) : (ti->tm_mon + 1);

    rtc.adjust(DateTime(year, month, date, 
            timeClient.getHours(), 
            timeClient.getMinutes(),
            timeClient.getSeconds()));
    _now = rtc.now();
    change_state_time = _now;

    #ifndef RELEASE
        Serial.print("Year: ");
        Serial.println(year);
        Serial.print("Datetime after NTP sync: ");
    #endif
    displayTime();
}

void displayTime(){
    #ifndef RELEASE
        Serial.print(rtc.now().year(), DEC);
        Serial.print('/');
        Serial.print(rtc.now().month(), DEC);
        Serial.print('/');
        Serial.print(rtc.now().day(), DEC);
        Serial.print("  ");
        Serial.print(rtc.now().hour(), DEC);
        Serial.print(':');
        Serial.print(rtc.now().minute(), DEC);
        Serial.print(':');
        Serial.print(rtc.now().second(), DEC);
        Serial.println();
    #endif
}