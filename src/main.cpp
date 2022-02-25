#include <Arduino.h>
#include <SD.h>
#include "config.h"
#include "logging.h"
#include "server.h"
#include "signals.h"
#include "common.h"
//#include "ftp.h"

#define T_STACK_SIZE 10000
#define T_LOOP_NAME "loop1"
#define T_SIGNAL_NAME "signal"

TaskHandle_t MainTask;
TaskHandle_t SignalTask;

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
      f.print("\"kVD\": 22, ");
      f.print("\"kI\": 30.3, ");
      f.print("\"cnom\": 6,");
      f.println("}");
      f.close();
    }
  #endif

  SetupErrors();
  InitSD();
  SetupScreen();
  SetupClock();
  SetupMotohours();
  GetCredentials();
  SetupServer();
  delay(2000);

  xTaskCreatePinnedToCore(loop1, T_LOOP_NAME, T_STACK_SIZE, NULL, 1, &MainTask, 0);
  xTaskCreatePinnedToCore(signalling, T_SIGNAL_NAME, T_STACK_SIZE, NULL, 1, &SignalTask, 1);
}

void loop() {
}

void loop1(void *param){
   for(;;){
     if(millis() - timer >= 100){
        timer  = millis();
        millisec = millis() - get_average_timer;
        ReadSensors();
        CheckErrors();
      }

      if(millis() - get_average_timer >= 1000){
        get_average_timer = millis();
        GetDateTime();
        GetSecondAverage();
        UpdateScreen();
      }

      if(millis() - logtimer >= 5000){
        logtimer = millis();
        LogData(); 
        LogError();
      }

      if(millis() - mototimer >= 60000){
        mototimer = millis();
        SaveMoto();
        GetMinuteAverageCurrent();
      }
   }
}

void signalling(void *param){
    for(;;){
      if(bitRead(ERROR_CODE, ERROR_LOW_VOLTAGE)) RunAlertLowVoltage();
    }
}

void GetCredentials(){
    File f = SD.open("/config.json", FILE_READ);
    if(f){
        DynamicJsonDocument doc(1024);
        String s = f.readString();
        deserializeJson(doc, s);
        JsonObject obj = doc.as<JsonObject>();
        String s1 = obj["ssid"];
        String s2 = obj["password"];
        net_ssid = s1.c_str();
        net_password = s2.c_str();
        f.close();
        
        #ifndef RELEASE
          Serial.print("Connecting to ");
          Serial.print(net_ssid);
          Serial.print(" with password ");
          Serial.println(net_password);
        #endif

        WiFi.begin(net_ssid, net_password);

        lcd.setTextSize(0);
        lcd.setTextColor(1);
        uint8_t n = 0;

        while (WiFi.status() != WL_CONNECTED) 
        {
          delay(1000);
          lcd.clearDisplay();
          lcd.setCursor(1, 1);
          lcd.print("Network: ");
          lcd.print(net_ssid);
          lcd.setCursor(1, 10);
          for(uint8_t i = 0; i < n; i++) lcd.print(".");
          n++;
          if(n > 5) n = 0;
          lcd.display();

          #ifndef RELEASE
            Serial.print(".");
          #endif
        }
        #ifndef RELEASE
          Serial.println("");
          Serial.println("WiFi connected..!");
          Serial.print("Got IP: ");
        #endif 

        lcd.clearDisplay();
        lcd.setCursor(1, 1);
        lcd.print("Network: ");
        lcd.print(net_ssid);
        lcd.setCursor(1, 10);
        lcd.print("Wifi connected!");
    
        timeClient.begin();
        timeClient.setTimeOffset(10800);
        timeClient.update();
        SyncTime();
    }
}