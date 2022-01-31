#include <Arduino.h>
#include <SD.h>
#include "config.h"
#include "logging.h"
#include "screen.h"
#include "errors.h"
#include "clock.h"
#include "server.h"
#include "sensors.h"
#include "moto.h"

TaskHandle_t SensorsTask;
TaskHandle_t LogTask;
TaskHandle_t ServerTask;
TaskHandle_t ErrorsTask;
TaskHandle_t ScreenTask;
TaskHandle_t MotoTask;

Loggable logger;
ErrorHandler error;
Screen screen(OLED_SDA, OLED_SCL);
ClockHandler clk;
ServerHandler server;
SensorsHandler sensors;
MotoHandler moto;

void setup() {
  #ifndef RELEASE
    Serial.begin(BAUD_RATE);
  #endif
  #ifdef NEW_CONFIG_JSON
    File f = SD.open("/config.json", FILE_WRITE);
    if(f){
      f.print("{");
      f.print("\"ssid\": \"Ceh\", ");
      f.print("\"password\": \"ceh@spil\", ");
      f.print("\"ip\": \"192.168.0.44\", ");
      f.print("\"gateway\": \"192.168.0.1\", ");
      f.print("\"subnet\": \"255.255.255.0\"");
      f.println("}");
      f.close();
    }
  #endif

  error.SetupErrors();
  logger.InitSD(&error);
  screen.SetupScreen();
  clk.SetupClock();
  moto.SetupMotohours();
  server.GetCredentials(&clk);
  server.SetupServer(&sensors, &moto, &clk, &error);
}

void loop() {
  // put your main code here, to run repeatedly:
}