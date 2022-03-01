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

void SaveStateTime(){
    File file = SD.open("/datafile.json", FILE_WRITE);
    String s = "";

    s += "{";
    s += "\"motohour\": \"";
    s += String(motohour);
    s += "\", ";
    s += "\"motomin\": \"";
    s += String(moto_time.minutes());
    s += "\", ";
    s += "\"motosec\": \"";
    s += String(moto_time.seconds());
    s += "\", ";
    s += "\"stateday\": \"";
    s += String(change_state_time.day());
    s += "\", ";
    s += "\"statemonth\": \"";
    s += String(change_state_time.month());
    s += "\", ";
    s += "\"stateyear\": \"";
    s += String(change_state_time.year());
    s += "\", ";
    s += "\"statehour\": \"";
    s += String(change_state_time.hour());
    s += "\", ";
    s += "\"statemin\": \"";
    s += String(change_state_time.minute());
    s += "\", ";
    s += "\"statesec\": \"";
    s += String(change_state_time.second());
    s += "\", ";
    s += "}";

    if(file){
       file.println(s);
    }
    file.close();
}