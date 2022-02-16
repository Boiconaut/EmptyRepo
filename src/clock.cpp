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
    sensors->SetChangeStateTime(&now());
}

void ClockHandler::GetDateTime(){
    _now = now();
}

DateTime* ClockHandler::GetTimeNow(){
    return &_now;
}

float ClockHandler::GetTemperature(){
    return getTemperature();
}

void ClockHandler::SyncTime(NTPClient *timeClient){
    time_t rawtime = timeClient->getEpochTime();
    struct tm * ti;
    ti = localtime (&rawtime);

    uint8_t date = (ti->tm_mday) < 10 ? 0 + (ti->tm_mday) : (ti->tm_mday);
    uint16_t year = ti->tm_year + 1900 - 2000;
    int month = (ti->tm_mon + 1) < 10 ? 0 + (ti->tm_mon + 1) : (ti->tm_mon + 1);

    _now = now();
    adjust(DateTime(year, month, date, 
            timeClient->getHours(), timeClient->getMinutes(), timeClient->getSeconds()));
}

uint8_t ClockHandler::GetDay(){
    return _now.day();
}

uint8_t ClockHandler::GetMonth(){
    return _now.month();
}

uint8_t ClockHandler::GetYear(){
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
