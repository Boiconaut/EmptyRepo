#ifndef COMMON_H
#define COMMON_H

#include "errors.h"
#include "clock.h"
#include "sensors.h"
#include "moto.h"

class MotoHandler;

ErrorHandler* getError();
ClockHandler* getClock();
SensorsHandler* getSensors();
MotoHandler* getMoto();

#endif