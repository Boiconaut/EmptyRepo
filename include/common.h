#ifndef COMMON_H
#define COMMON_H

#include "errors.h"
#include "clock.h"
#include "sensors.h"
#include "moto.h"
#include "screen.h"

void loop1(void *param);
void signalling(void *param);

void GetCredentials();

#endif