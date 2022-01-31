#ifndef LOGGER_H
#define LOGGER_H

#define PATH_ERROR 1
#define PATH_DATA 0
#define CHIP_SELECT 5

//#include "errors.h"

class ErrorHandler;

class Logger{
protected:
    void Log(String data, uint8_t path, ErrorHandler *error);
public:
    Logger();
    ~Logger();
};

#endif