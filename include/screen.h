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

class ErrorHandler;

class Screen : public Adafruit_SH1106 {
private:
    void Screen_1(ClockHandler *clk, SensorsHandler *sensors);
    void Screen_2(ErrorHandler *error, SensorsHandler *sensors);
public:
    Screen(uint8_t SDA, uint8_t SCL);
    void SetupScreen();
    void UpdateScreen(ClockHandler *clk, SensorsHandler *sensors, ErrorHandler *error);
};

#endif