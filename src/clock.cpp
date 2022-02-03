#include <Wire.h>
#include "clock.h"
#include "config.h"

ClockHandler::ClockHandler(){
    millisec = 0;
}

ClockHandler::~ClockHandler(){
    
}

void ClockHandler::SetupClock(){
    Wire.begin();
    Wire.beginTransmission(DS3231_I2C_ADDRESS); // 0x68 is DS3231 device address
    Wire.write(0x0E); //
    Wire.write(B00000000); //  EOSC-0-запуск генератора,BBSQW-0-SQW-вкл,CONV-1-измерение темп-ры,  RS2иRS1-частота SQW,INTCN-SQW-будильник или пила частотой RS1-RS2,A2IEиA1IE-прерывания по будильнику.
    Wire.write(B00000000); //  OSF-0-запуск генератора,0,0,0,EN32kHz,BSY-во время корректировки(автосброс),A2FиA1F - срабатывание будильника,
    Wire.endTransmission();
}

uint8_t ClockHandler::decToBcd(uint8_t val){
    return ((val / 10 * 16) + (val % 10));
}

void ClockHandler::watchConsole(){
    if (Serial.available()) {      
        if (Serial.read() == 84) {      
            getDateTime();
            Serial.println(" ");
        }
    }
}

uint8_t ClockHandler::getRegister(uint8_t regNo){
    Wire.beginTransmission(DS3231_I2C_ADDRESS); 
    Wire.write(regNo); 
    Wire.endTransmission();
    Wire.requestFrom(DS3231_I2C_ADDRESS, 1); 
    return  Wire.read();
}

void ClockHandler::setRegister(uint8_t regNo, uint8_t value){
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(regNo);
    Wire.write(value);
    Wire.endTransmission();
}

void ClockHandler::getDateTime(){
    Wire.beginTransmission(DS3231_I2C_ADDRESS); // 0x68 is DS3231 device address
    Wire.write(0x00); // start at register 0
    Wire.endTransmission();
    Wire.requestFrom(DS3231_I2C_ADDRESS, 7); // request seven bytes
    
    if(Wire.available()) {
        seconds = Wire.read(); // get seconds
        minutes = Wire.read(); // get minutes
        hours   = Wire.read();   // get hours
        day     = Wire.read();
        date    = Wire.read();
        month   = Wire.read(); //temp month
        year    = Wire.read();
        seconds = (((seconds & B11110000)>>4)*10 + (seconds & B00001111)); // convert BCD to decimal
        minutes = (((minutes & B11110000)>>4)*10 + (minutes & B00001111)); // convert BCD to decimal
        hours   = (((hours & B00110000)>>4)*10 + (hours & B00001111)); // convert BCD to decimal (assume 24 hour mode)
        day     = (day & B00000111); // 1-7
        date    = (((date & B00110000)>>4)*10 + (date & B00001111)); // 1-31
        month   = (((month & B00010000)>>4)*10 + (month & B00001111)); //msb7 is century overflow
        year    = (((year & B11110000)>>4)*10 + (year & B00001111));
    }
    else {
        #ifndef RELEASE
            Serial.print("No data from date and time cells");
        #endif
    }
    
    switch (day) {
        case 1:
        strcpy(weekDay, "Sun");
        break;
        case 2:
        strcpy(weekDay, "Mon");
        break;
        case 3:
        strcpy(weekDay, "Tue");
        break;
        case 4:
        strcpy(weekDay, "Wed");
        break;
        case 5:
        strcpy(weekDay, "Thu");
        break;
        case 6:
        strcpy(weekDay, "Fri");
        break;
        case 7:
        strcpy(weekDay, "Sat");
        break;
    }
}

String ClockHandler::getFormattedDate(NTPClient *timeClient){
    time_t rawtime = timeClient->getEpochTime();
    struct tm * ti;
    ti = localtime (&rawtime);

    uint16_t _year = ti->tm_year + 1900 - 2000;
    String yearStr = String(_year);

    uint8_t _month = ti->tm_mon + 1;
    String monthStr = _month < 10 ? "0" + String(_month) : String(_month);

    uint8_t _day = ti->tm_mday;
    String dayStr = _day < 10 ? "0" + String(_day) : String(_day);

    uint8_t _hours = ti->tm_hour;
    String hoursStr = _hours < 10 ? "0" + String(_hours) : String(_hours);

    uint8_t _minutes = ti->tm_min;
    String minuteStr = _minutes < 10 ? "0" + String(_minutes) : String(_minutes);

    uint8_t _seconds = ti->tm_sec;
    String secondStr = _seconds < 10 ? "0" + String(_seconds) : String(_seconds);

    return "T" + secondStr + minuteStr + hoursStr + "0" + dayStr + monthStr + yearStr;
}

uint16_t ClockHandler::getYear(NTPClient *timeClient){
    time_t rawtime = timeClient->getEpochTime();
    struct tm * ti;
    ti = localtime (&rawtime);
    int year = ti->tm_year + 1900 - 2000;
    
    return year;
}

uint16_t ClockHandler::getMonth(NTPClient *timeClient){
    time_t rawtime = timeClient->getEpochTime();
    struct tm * ti;
    ti = localtime (&rawtime);
    int month = (ti->tm_mon + 1) < 10 ? 0 + (ti->tm_mon + 1) : (ti->tm_mon + 1);
    
    return month;
}

uint16_t ClockHandler::getDate(NTPClient *timeClient){
    time_t rawtime = timeClient->getEpochTime();
    struct tm * ti;
    ti = localtime (&rawtime);
    int day = (ti->tm_mday) < 10 ? 0 + (ti->tm_mday) : (ti->tm_mday);
    
    return day;
}

void ClockHandler::GetDateTime(){
    watchConsole();
    getDateTime();
}

void ClockHandler::SetDateTime(uint8_t _date, uint8_t _month, uint8_t _year, 
                     uint8_t _hours, uint8_t _minutes, uint8_t _seconds){
    date = _date;
    month = _month;
    year = _year;
    hours = _hours;
    minutes = _minutes;
    seconds = _seconds;
}

float ClockHandler::GetTemperature(){
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0x11);
    Wire.endTransmission();
    Wire.requestFrom(DS3231_I2C_ADDRESS, 2);
    
    if(Wire.available()) {
        tMSB = Wire.read(); //2's complement int portion
        tLSB = Wire.read(); //fraction portion
        
        temp3231 = (tMSB & B01111111); //do 2's math on Tmsb
        temp3231 += ( (tLSB >> 6) * 0.25 ); //only care about bits 7 & 8
    }
    else {
        Serial.print("No data from 3231temp");
    }
        
    return temp3231;
}

void ClockHandler::SyncTime(NTPClient *timeClient){
    seconds = (byte) timeClient->getSeconds();
    minutes = (byte) timeClient->getMinutes();
    hours   = (byte) timeClient->getHours();
    day     = (byte) timeClient->getDay();
    date    = (byte) getDate(timeClient);
    month   = (byte) getMonth(timeClient);
    year    = (byte) getYear(timeClient);
    
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0x00);
    Wire.write(decToBcd(seconds)); //0x00
    Wire.write(decToBcd(minutes)); //0x01
    Wire.write(decToBcd(hours));   //0x02
    Wire.write(decToBcd(day));     //0x03 
    Wire.write(decToBcd(date));    //0x04
    Wire.write(decToBcd(month));   //0x05
    Wire.write(decToBcd(year));    //0x06
    Wire.endTransmission();
    Wire.beginTransmission(DS3231_I2C_ADDRESS); // 0x68 is DS3231 device address
    Wire.write(0x0E); //
    Wire.write(B00000000); //  EOSC-0-запуск генератора,BBSQW-0-SQW-вкл,CONV-1-измерение темп-ры,  RS2иRS1-частота SQW,INTCN-SQW-будильник или пила частотой RS1-RS2,A2IEиA1IE-прерывания по будильнику.
    Wire.write(B00000000); //  OSF-0-запуск генератора,0,0,0,EN32kHz,BSY-во время корректировки(автосброс),A2FиA1F - срабатывание будильника
    Wire.write(B10001000);     //to be commented
    Wire.endTransmission();
}

uint8_t ClockHandler::GetDay(){
    return date;
}

uint8_t ClockHandler::GetMonth(){
    return month;
}

uint8_t ClockHandler::GetYear(){
    return year;
}

uint8_t ClockHandler::GetHours(){
    return hours;
}

uint8_t ClockHandler::GetMinutes(){
    return minutes;
}

uint8_t ClockHandler::GetSeconds(){
    return seconds;
}

uint16_t ClockHandler::GetMillis(){
    return millisec;
}

void ClockHandler::SetMillis(uint16_t m){
    millisec = m;
}
