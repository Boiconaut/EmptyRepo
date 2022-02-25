#include "logging.h"

String dataString = "";

void InitSD(){
   if(!SD.begin(CHIP_SELECT)){
     #ifndef RELEASE
       Serial.print("No SD card");
     #endif
     ERROR_CODE |= (1 << ERROR_NO_SD);
  }
}

void LogData(){
    String dataString = "";
    
    dataString += String(_now.day());
    dataString += "-";
    dataString += String(_now.month());
    dataString += "-";
    dataString += String(_now.year());
    dataString += " ";
    dataString += String(_now.hour());
    dataString += ":";
    dataString += String(_now.minute());
    dataString += ":";
    dataString += String(_now.second());
    dataString += "  ";
    
    dataString += "U = ";
    dataString += String(voltage);
    dataString += ", I = ";
    dataString += String(current);
    dataString += ", P = ";
    dataString += String(power);
    dataString += ",  SOC = ";
    dataString += String(SOC);
    dataString += "%";
   
    File file = SD.open("/datalog.txt", FILE_APPEND);
   
    if (file) {
      file.println(dataString);
      ERROR_CODE &= ~(1 << ERROR_NO_SD);
    }
    else{
      ERROR_CODE |= (1 << ERROR_NO_SD);
    }
    file.close();
}