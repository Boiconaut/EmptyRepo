#include <SD.h>
#include "logger.h"
#include "errors.h"

Logger::Logger(){

}

Logger::~Logger(){
  
}

void Logger::Log(String data, uint8_t path, ErrorHandler *error){
    File file;
    if(path == PATH_DATA) file = SD.open("/datalog.txt", FILE_APPEND);
    else if(path == PATH_ERROR) file = SD.open("/errors.txt", FILE_APPEND);
   
    if (file) {
      file.println(data);
      error->ERROR_CODE &= ~(1 << ERROR_NO_SD);
    }
    else{
      error->ERROR_CODE |= (1 << ERROR_NO_SD);
    }
    file.close();
}