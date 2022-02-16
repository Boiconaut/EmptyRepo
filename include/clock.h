#ifndef CLOCK_H
#define CLOCK_H

#define DS3231_I2C_ADDRESS 0x68
#include <NTPClient.h>
#include <Arduino.h>
#include <RTCLib.h>
#include "common.h"

class ClockHandler : RTC_DS3231{
private:
    uint16_t millisec;
    DateTime _now;
public:
    ClockHandler();
    ~ClockHandler();
    
    void SetupClock(SensorsHandler *sensors);
    void GetDateTime();
    float GetTemperature();
    void SyncTime(NTPClient *timeClient);
    DateTime* GetTimeNow();
    
    uint8_t GetDay();
    uint8_t GetMonth();
    uint8_t GetYear();
    uint8_t GetHours();
    uint8_t GetMinutes();
    uint8_t GetSeconds();
    uint16_t GetMillis();
    void SetMillis(uint16_t m);
};

#endif