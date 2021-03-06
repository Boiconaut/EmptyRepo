#ifndef SERVER_H
#define SERVER_H

#include <WiFi.h>
#include <WiFiUdp.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <NTPClient.h>
#include <ArduinoJson.h>
#include "common.h"

extern const char *user_login;
extern const char *user_password;
extern const char *net_ssid;
extern const char *net_password;

extern IPAddress ip;      
extern IPAddress gateway;   
extern IPAddress subnet;  

extern AsyncWebServer server;
extern WiFiUDP ntpUDP;               
extern NTPClient timeClient;
extern DynamicJsonDocument json; 

extern String valid_token;
extern boolean IsAuth;
extern boolean IsAdmin;

String getAuthToken();
void handle_NotFound(AsyncWebServerRequest *request); 
void handle_onAuth(AsyncWebServerRequest *request, String data);
void handle_Prognosis(AsyncWebServerRequest *request, String data);
void handle_Voltage(AsyncWebServerRequest *request, String data);
void handle_CalibrateVoltage(AsyncWebServerRequest *request, String data);
void handle_CalibrateCurrent(AsyncWebServerRequest *request, String data);
void handle_SetCapacity(AsyncWebServerRequest *request, String data);
void assembleData();

String htmlData();
String htmlDataAdmin();
String htmlLogin();
String htmlInvalid();
String htmlNotFound();

void SetupServer();

#endif