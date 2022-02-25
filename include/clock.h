#ifndef CLOCK_H
#define CLOCK_H

#define DS3231_I2C_ADDRESS 0x68
#include <NTPClient.h>
#include <Arduino.h>
#include <RTCLib.h>
#include "sensors.h"
#include "common.h"

extern uint16_t millisec;
extern DateTime _now; 
void displayTime();
extern RTC_DS3231 rtc;

void SetupClock();
void GetDateTime();
float GetTemperature();
void SyncTime();

#endif