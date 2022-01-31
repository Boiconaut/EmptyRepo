#include <SD.h>
#include "logging.h"
#include "errors.h"
#include "config.h"

Loggable::Loggable(){
    dataString = "";
}

Loggable::~Loggable(){
  
}

void Loggable::InitSD(ErrorHandler *error){
   if(!SD.begin(CHIP_SELECT)){
     #ifndef RELEASE
       Serial.print("No SD card");
     #endif
     error->ERROR_CODE |= (1 << ERROR_NO_SD);
  }
}

void Loggable::LogData(ClockHandler *clk, SensorsHandler *sensors, ErrorHandler *error){
    String dataString = "";
    
    dataString += String(clk->GetDay());
    dataString += "-";
    dataString += String(clk->GetMonth());
    dataString += "-";
    dataString += String(clk->GetYear());
    dataString += " ";
    dataString += String(clk->GetHours());
    dataString += ":";
    dataString += String(clk->GetMinutes());
    dataString += ":";
    dataString += String(clk->GetSeconds());
    dataString += "  ";
    
    dataString += "U = ";
    dataString += String(sensors->GetVoltage());
    dataString += ", I = ";
    dataString += String(sensors->GetCurrent());
    dataString += ", P = ";
    dataString += String(sensors->GetPower());
    dataString += ",  SOC = ";
    dataString += String(sensors->GetSOC());
    dataString += "%";
   
   Log(dataString, 0, error);
}