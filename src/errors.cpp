#include <SD.h>
#include "errors.h"

ErrorHandler::ErrorHandler(){
    error_string = "";
}

ErrorHandler::~ErrorHandler(){
    
}

void ErrorHandler::addMessage(String mes){
    error_string += mes;
}

void ErrorHandler::clear(){
    error_string = "";
}

void ErrorHandler::SetupErrors(){
    pinMode(tonePin, OUTPUT);
    pinMode(RedLED, OUTPUT);
    pinMode(BlueLED, OUTPUT);
    ERROR_CODE = B00000000;
}

void ErrorHandler::CheckErrors(ClockHandler *clk, SensorsHandler *sensors){
    error_string += String(clk->GetDay());
    error_string += "-";
    error_string += String(clk->GetMonth());
    error_string += "-";
    error_string += String(clk->GetYear());
    error_string += " ";
    error_string += String(clk->GetHours());
    error_string += ":";
    error_string += String(clk->GetMinutes());
    error_string += ":";
    error_string += String(clk->GetSeconds());
    error_string += ".";
    error_string += String(clk->GetMillis());
    error_string += "  ";

    if(sensors->GetMomentVoltage() < sensors->v20) ERROR_CODE |= (1 << ERROR_LOW_VOLTAGE);
    if(sensors->GetMomentVoltage() >= sensors->v20) ERROR_CODE &= ~(1 << ERROR_LOW_VOLTAGE);
    if(sensors->GetMomentCurrent() > 200) ERROR_CODE |= (1 << ERROR_OVERCURRENT);
    if(sensors->GetMomentCurrent() <= 200) ERROR_CODE &= ~(1 << ERROR_OVERCURRENT);

    if(bitRead(ERROR_CODE, ERROR_LOW_VOLTAGE)){
        error_string += "Error: low voltage! U = ";
        error_string += String(sensors->GetMomentVoltage());
        error_string += " V";
        error_string += " ";
    }
    if(bitRead(ERROR_CODE, ERROR_OVERCURRENT)){
        error_string += "Error: overcurrent! I = ";
        error_string += String(sensors->GetMomentCurrent());
        error_string += " A";
        error_string += " ";
    }
    error_string += "\n";
}

void ErrorHandler::LogError(ErrorHandler *error){
    Log(error_string, 1, error);
    clear();
}