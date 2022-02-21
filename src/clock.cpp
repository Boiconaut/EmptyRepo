#include <Wire.h>
#include "clock.h"
#include "config.h"

ClockHandler::ClockHandler() : RTC_DS3231(){
    millisec = 0;
}

ClockHandler::~ClockHandler(){
    
}

void ClockHandler::SetupClock(SensorsHandler *sensors){
    if (! begin()) {
        #ifndef RELEASE
            Serial.println("Couldn't find RTC");
            Serial.flush();
        #endif
        while (1) delay(10);
    }

    //adjust(DateTime(F(__DATE__), F(__TIME__)));
    #ifndef RELEASE
        Serial.print("Datetime in clock setup: ");
    #endif
    _displayTime();
}

void ClockHandler::GetDateTime(NTPClient *timeClient, SensorsHandler *sensors){
    if(now().day() > 31 || now().hour() > 24 || now().minute() > 59 || now().second() > 59){
        timeClient->update();
        SyncTime(timeClient, sensors);
        #ifndef RELEASE
            Serial.println("Time error! RTC resync");
        #endif
    }
    else {
        _now = now();
    }
}

DateTime* ClockHandler::GetTimeNow(){
    return &_now;
}

float ClockHandler::GetTemperature(){
    return getTemperature();
}

void ClockHandler::SyncTime(NTPClient *timeClient, SensorsHandler *sensors){
    time_t rawtime = timeClient->getEpochTime();
    struct tm * ti;
    ti = localtime (&rawtime);

    uint8_t date = (ti->tm_mday) < 10 ? 0 + (ti->tm_mday) : (ti->tm_mday);
    uint16_t year = ti->tm_year + 1900 - 2000;
    uint8_t month = (ti->tm_mon + 1) < 10 ? 0 + (ti->tm_mon + 1) : (ti->tm_mon + 1);

    adjust(DateTime(year, month, date, 
            timeClient->getHours(), timeClient->getMinutes(), timeClient->getSeconds()));
    _now = now();
    sensors->SetChangeStateTime(&_now);

    #ifndef RELEASE
        Serial.print("Year: ");
        Serial.println(year);
        Serial.print("Datetime after NTP sync: ");
    #endif
    _displayTime();
}

uint8_t ClockHandler::GetDay(){
    return _now.day();
}

uint8_t ClockHandler::GetMonth(){
    return _now.month();
}

uint16_t ClockHandler::GetYear(){
    return _now.year();
}

uint8_t ClockHandler::GetHours(){
    return _now.hour();
}

uint8_t ClockHandler::GetMinutes(){
    return _now.minute();
}

uint8_t ClockHandler::GetSeconds(){
    return _now.second();
}

uint16_t ClockHandler::GetMillis(){
    return millisec;
}

void ClockHandler::SetMillis(uint16_t m){
    millisec = m;
}

void ClockHandler::_displayTime(){
    #ifndef RELEASE
        Serial.print(now().year(), DEC);
        Serial.print('/');
        Serial.print(now().month(), DEC);
        Serial.print('/');
        Serial.print(now().day(), DEC);
        Serial.print("  ");
        Serial.print(now().hour(), DEC);
        Serial.print(':');
        Serial.print(now().minute(), DEC);
        Serial.print(':');
        Serial.print(now().second(), DEC);
        Serial.println();
    #endif
}