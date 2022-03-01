#ifndef LOGGING_H
#define LOGGING_H

#include <SD.h>
#include <ArduinoJson.h>
#include "clock.h"
#include "errors.h"
#include "common.h"
#include "config.h"
//#include "ftp.h"

#define CHIP_SELECT 5

extern String dataString;

void InitSD();
void LogData();
void SaveStateTime();

#endif