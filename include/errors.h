#ifndef ERRORS_H
#define ERRORS_H

#include "clock.h"
#include "sensors.h"
#include "logger.h"

#define tonePin 17            
#define RedLED 10             
#define BlueLED 11   

#define ERROR_LOW_VOLTAGE 0   
#define ERROR_OVERCURRENT 1   
#define ERROR_NO_SD 2   

class Logger;

class ErrorHandler : Logger {
private:
    String error_string;
    void addMessage(String mes);
    void clear();
public:
    uint16_t ERROR_CODE;
    ErrorHandler();
    ~ErrorHandler();
    void SetupErrors();
    void CheckErrors(ClockHandler *clk, SensorsHandler *sensors);
    void LogError();
};

#endif