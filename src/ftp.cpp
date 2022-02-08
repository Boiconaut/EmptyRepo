#include <Arduino.h>
#include "ftp.h"

FTPHandler::FTPHandler(char* _serverAdress, char* _userName, char* _passWord, 
            uint16_t _timeout, uint8_t _verbose) : 
            ESP32_FTPClient(_serverAdress, _userName, _passWord, _timeout = 10000, _verbose = 1){

}