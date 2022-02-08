#ifndef FTP_H
#define FTP_H

#include <ESP32_FTPClient.h>

#define FTP_SERVER ""
#define FTP_USER ""
#define FTP_PASS ""
#define FTP_TIMEOUT 5000
#define FTP_VERBOSE 2

class FTPHandler : ESP32_FTPClient {
public:
    FTPHandler(char* _serverAdress, char* _userName, char* _passWord, 
                uint16_t _timeout = 10000, uint8_t _verbose = 1);
};

#endif