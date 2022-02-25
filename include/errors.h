#ifndef ERRORS_H
#define ERRORS_H

#include <SD.h>
#include "clock.h"
#include "sensors.h"

#define tonePin 17            
#define RedLED 10             
#define BlueLED 11   

#define ERROR_LOW_VOLTAGE 0   
#define ERROR_OVERCURRENT 1   
#define ERROR_NO_SD 2   

extern String error_string;
extern uint16_t ERROR_CODE;

void SetupErrors();
void CheckErrors();
void LogError();

#endif