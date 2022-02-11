#ifndef LOGGING_H
#define LOGGING_H

#include "clock.h"
#include "sensors.h"
#include "errors.h"
#include "logger.h"
//#include "ftp.h"

class ErrorHandler;

class Loggable : Logger {
private:
    String dataString;
public:
    Loggable();
    ~Loggable();
    void InitSD(ErrorHandler *error);
    void LogData(ClockHandler *clk, SensorsHandler *sensors, ErrorHandler *error);
};

#endif