#ifndef SIGNALS_H
#define SIGNALS_H

#include <Arduino.h>

#define tonePin 17            
#define RedLED 10             
#define BlueLED 11 
#define FREQUENCY 3725
#define DURATION 500
#define INTERVAL 500

class SignalHandler {
private:
    void tone(uint8_t pin, uint16_t frequency, uint32_t duration);
public:
    void RunAlertLowVoltage();
};

#endif