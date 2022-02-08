#include <Arduino.h>
#include <SD.h>
#include "config.h"
#include "logging.h"
#include "screen.h"
//#include "errors.h"
//#include "clock.h"
#include "server.h"
//#include "sensors.h"
//#include "moto.h"
#include "signals.h"
#include "common.h"
#include "ftp.h"

#define T_STACK_SIZE 10000
#define T_LOOP_NAME "loop1"
#define T_SIGNAL_NAME "signal"

TaskHandle_t MainTask;
TaskHandle_t SignalTask;

Loggable logger;
ErrorHandler error;
Screen screen(OLED_SDA, OLED_SCL);
ClockHandler clk;
ServerHandler server;
SensorsHandler sensors;
MotoHandler moto;
SignalHandler signal;
FTPHandler ftp(FTP_SERVER, FTP_USER, FTP_PASS, FTP_TIMEOUT, FTP_VERBOSE);

void loop1(void *param);
void signalling(void *param);

///////////////////////////
//Lines to be deleted after adding multithreading algorithm
unsigned long timer = 0;
unsigned long logtimer = 0;
unsigned long mototimer  = 0;
unsigned long error_timer = 0;
unsigned long get_average_timer = 0;
///////////////////////////

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
  server.GetCredentials(&clk, &screen);
  server.SetupServer(&screen);
  delay(2000);

  xTaskCreatePinnedToCore(loop1, T_LOOP_NAME, T_STACK_SIZE, NULL, 1, &MainTask, 0);
  xTaskCreatePinnedToCore(signalling, T_SIGNAL_NAME, T_STACK_SIZE, NULL, 1, &MainTask, 1);
}

void loop() {
  //////////////////////////////////////
  // This code is to be deleted. There will be no loop in multithreading app
  /*if(millis() - timer >= 100){
    timer  = millis();
    clk.SetMillis(millis() - get_average_timer);
    sensors.ReadSensors();
  }

  if(millis() - get_average_timer >= 1000){
    get_average_timer = millis();
    clk.GetDateTime();
    sensors.GetSecondAverage();
    screen.UpdateScreen(&clk, &sensors, &error);
  }

  if(millis() - logtimer >= 5000){
    logtimer = millis();
    logger.LogData(&clk, &sensors, &error);
    error.LogError();
  }

  if(millis() - mototimer >= 60000){
    mototimer = millis();
    moto.Save();
    sensors.GetMinuteAverageCurrent();
  }*/
  //////////////////////////////////////////
}

void loop1(void *param){
   for(;;){
     if(millis() - timer >= 100){
        timer  = millis();
        clk.SetMillis(millis() - get_average_timer);
        sensors.ReadSensors();
        error.CheckErrors(&clk, &sensors);
      }

      if(millis() - get_average_timer >= 1000){
        get_average_timer = millis();
        clk.GetDateTime();
        sensors.GetSecondAverage();
        screen.UpdateScreen(&clk, &sensors, &error);
      }

      if(millis() - logtimer >= 5000){
        logtimer = millis();
        logger.LogData(&clk, &sensors, &error);
        error.LogError();
      }

      if(millis() - mototimer >= 60000){
        mototimer = millis();
        moto.Save();
        sensors.GetMinuteAverageCurrent();
      }
   }
}

void signalling(void *param){
    for(;;){
      if(bitRead(error.ERROR_CODE, ERROR_LOW_VOLTAGE)) signal.RunAlertLowVoltage();
    }
}

ErrorHandler* getError(){
  return &error;
}

ClockHandler* getClock(){
  return &clk;
}

SensorsHandler* getSensors(){
  return &sensors;
}

MotoHandler* getMoto(){
  return &moto;
}