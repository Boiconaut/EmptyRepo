#include "errors.h"

String error_string = "";
uint16_t ERROR_CODE = B00000000;

void SetupErrors(){
  pinMode(tonePin, OUTPUT);
  pinMode(RedLED, OUTPUT);
  pinMode(BlueLED, OUTPUT);
}

void CheckErrors(){
  error_string += String(_now.day());
  error_string += "-";
  error_string += String(_now.month());
  error_string += "-";
  error_string += String(_now.year());
  error_string += " ";
  error_string += String(_now.hour());
  error_string += ":";
  error_string += String(_now.minute());
  error_string += ":";
  error_string += String(_now.second());
  error_string += ".";
  error_string += String(millisec);
  error_string += "  ";

  if(moment_voltage < v20) ERROR_CODE |= (1 << ERROR_LOW_VOLTAGE);
  if(moment_voltage >= v20) ERROR_CODE &= ~(1 << ERROR_LOW_VOLTAGE);
  if(moment_current > 200) ERROR_CODE |= (1 << ERROR_OVERCURRENT);
  if(moment_current <= 200) ERROR_CODE &= ~(1 << ERROR_OVERCURRENT);

  if(bitRead(ERROR_CODE, ERROR_LOW_VOLTAGE)){
      error_string += "Error: low voltage! U = ";
      error_string += String(moment_voltage);
      error_string += " V";
      error_string += " ";
    }
    if(bitRead(ERROR_CODE, ERROR_OVERCURRENT)){
      error_string += "Error: overcurrent! I = ";
      error_string += String(moment_current);
      error_string += " A";
      error_string += " ";
    }
    error_string += "\n";
}

void LogError(){
  File errors_file = SD.open("/errors.txt", FILE_APPEND);

  if(error_string.length() > 2){
    if(errors_file){
        errors_file.println(error_string);
        ERROR_CODE &= ~(1 << ERROR_NO_SD);
      }
      else{
        ERROR_CODE |= (1 << ERROR_NO_SD);
      }
      errors_file.close();
      error_string = "\n";
  }
}