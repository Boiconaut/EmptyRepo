#ifndef SERVER_H
#define SERVER_H

#include <WiFi.h>
#include <WiFiUdp.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <NTPClient.h>
#include <ArduinoJson.h>
#include "sensors.h"
#include "moto.h"
#include "clock.h"
#include "errors.h"

class ServerHandler{
private:
    const char *user_login;
    const char *user_password;
    const char *net_ssid;
    const char *net_password;

    IPAddress ip = IPAddress(192,168,0,44);      
    IPAddress gateway = IPAddress(192,168,0,1);   
    IPAddress subnet = IPAddress(255,255,255,0);  

    AsyncWebServer server = AsyncWebServer(80);
    WiFiUDP ntpUDP;               
    NTPClient timeClient = NTPClient(ntpUDP);
    DynamicJsonDocument json = DynamicJsonDocument(2048); 

    String valid_token;
    boolean IsAuth;

    String getAuthToken();
    void handle_NotFound(AsyncWebServerRequest *request);
    void handle_onAuth(AsyncWebServerRequest *request, String data);
    void handle_Prognosis(AsyncWebServerRequest *request, String data, SensorsHandler *sensors);
    void handle_Voltage(AsyncWebServerRequest *request, String data, SensorsHandler *sensors);
    void assembleData(SensorsHandler *sensors, MotoHandler *moto, ClockHandler *clk, ErrorHandler *error);

    String htmlData();
    String htmlLogin();
    String htmlInvalid();
    String htmlNotFound();
public:
    ServerHandler();
    ~ServerHandler();
    void GetCredentials(ClockHandler *clk);
    void SetupServer(SensorsHandler *sensors, MotoHandler *moto, ClockHandler *clk, ErrorHandler *error);
};

#endif