#include <SD.h>
#include "logger.h"
#include "errors.h"

Logger::Logger(){
  // : FTPHandler(FTP_SERVER, FTP_USER, FTP_PASS, FTP_TIMEOUT, FTP_VERBOSE)
}

Logger::~Logger(){
  
}

void Logger::Log(String data, uint8_t path, ErrorHandler *error){
    File file;
    /*if(path == PATH_DATA) file = SD.open("/datalog.txt", FILE_APPEND);
    else if(path == PATH_ERROR) file = SD.open("/errors.txt", FILE_APPEND);*/
    const char* filename;
    if(path == PATH_DATA) filename = "/datalog.txt";
    else if(path = PATH_ERROR) filename = "/errors.txt";
    file = SD.open(filename, FILE_APPEND);
   
    if (file) {
      file.println(data);
      error->ERROR_CODE &= ~(1 << ERROR_NO_SD);
    }
    else{
      error->ERROR_CODE |= (1 << ERROR_NO_SD);
    }
    file.close();
}