#ifndef CLOCK_H
#define CLOCK_H

#define DS3231_I2C_ADDRESS 0x68
#include <NTPClient.h>
#include <Arduino.h>

class ClockHandler{
private:
    uint8_t seconds, minutes, hours, day, date, month, year;  
    uint16_t millisec;
    uint8_t sec_before; 
    char weekDay[4];  
    uint8_t tMSB, tLSB;
    float temp3231;

    uint8_t decToBcd(uint8_t val);
    void watchConsole();
    uint8_t getRegister(uint8_t regNo);
    void setRegister(uint8_t regNo, uint8_t value);
    void getDateTime();
    String getFormattedDate(NTPClient *timeClient);
    uint16_t getYear(NTPClient *timeClient);
    uint16_t getMonth(NTPClient *timeClient);
    uint16_t getDate(NTPClient *timeClient);
public:
    ClockHandler();
    ~ClockHandler();
    
    void SetupClock();
    uint16_t GetDateTime(NTPClient *timeClient);
    void SetDateTime(uint8_t _date, uint8_t _month, uint8_t _year, 
                     uint8_t _hours, uint8_t _minutes, uint8_t _seconds);
    float GetTemperature();
    void SyncTime(NTPClient *timeClient);
    
    uint8_t GetDay();
    uint8_t GetMonth();
    uint8_t GetYear();
    uint8_t GetHours();
    uint8_t GetMinutes();
    uint8_t GetSeconds();
    uint16_t GetMillis();
};

#endif