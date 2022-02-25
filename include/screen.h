#ifndef SCREEN_H
#define SCREEN_H

#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include "errors.h"
#include "sensors.h"
#include "clock.h"

#define OLED_SDA 21  
#define OLED_SCL 22  
#define SCREEN_ADDR 0x3C

void Screen_1(); 
void Screen_2();

extern Adafruit_SH1106 lcd;
void SetupScreen();
void UpdateScreen();

#endif