#include "signals.h"

void SignalHandler::tone(uint8_t pin, uint16_t frequency, uint32_t duration){
    uint32_t t = millis();
    uint32_t i = 0;
    float interval = 1000000 / float(frequency);
    
    while(millis() - t <= duration){
        if(micros() - i > interval / 2 && micros() - i < interval){
            digitalWrite(pin, HIGH);
        }
        if(micros() - i >= interval){
            i = micros();
            digitalWrite(pin, LOW);
        }
    }
}

void SignalHandler::RunAlertLowVoltage(){
    tone(tonePin, FREQUENCY, DURATION);
    delay(INTERVAL);
}