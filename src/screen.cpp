#include <WiFi.h>
#include "screen.h"

Adafruit_SH1106 lcd(OLED_SDA, OLED_SCL);

void Screen_1(){
    lcd.setCursor(1, 1);
    if(_now.day() < 10) lcd.print("0");
    lcd.print(_now.day());
    lcd.print("-");
    if(_now.month() < 10) lcd.print("0");
    lcd.print(_now.month());
    lcd.print("-");
    lcd.print(_now.year());
    lcd.print(" ");
    if(_now.hour() < 10) lcd.print("0");
    lcd.print(_now.hour());
    lcd.print(":");
    if(_now.minute() < 10) lcd.print("0");
    lcd.print(_now.minute());
    lcd.print(":");
    if(_now.second() < 10) lcd.print("0");
    lcd.print(_now.second());

    lcd.setCursor(1, 10);
    lcd.print("V=");
    lcd.print(voltage);
    lcd.print(" V");

    lcd.setCursor(1, 19);
    lcd.print("I=");
    lcd.print(current);
    lcd.print(" A");

    lcd.setCursor(1, 28);
    lcd.print("P=");
    lcd.print(power);
    lcd.print(" W");

    uint8_t level = SOC / 6;
    for(uint8_t i = 0; i < level; i++){
      lcd.fillRect(i * 8, 37, 7, 23, 1);
    }
}

void Screen_2(){
    lcd.setCursor(1, 1);
    lcd.print("Nominal: ");
    lcd.print(NOM_VOLTAGE);
    lcd.print(" V  ");
    lcd.print(NOM_CAPACITY);
    lcd.print(" Ah");

    lcd.setCursor(1, 10);
    if(STATE == 1) lcd.print("On charge: ");
    else if(STATE == 2) lcd.print("On discharge: ");
    else lcd.print("Relaxing: ");
    String h = state_time.hours() < 0 ? "0" + String(state_time.hours()) : String(state_time.hours());
    String m = state_time.minutes() < 0 ? "0" + String(state_time.minutes()) : String(state_time.minutes());
    String s = state_time.seconds() < 0 ? "0" + String(state_time.seconds()) : String(state_time.seconds());
    String state_time = h + ":" + m + ":" + s;
    lcd.print(state_time);

    lcd.setCursor(1, 19);
    lcd.print("Coulomb: ");
    lcd.print(_capacity);
    lcd.print(" mAh");

    lcd.setCursor(1, 28);
    WiFi.status() == WL_CONNECTED ? lcd.print("Wifi connected") : lcd.print("No Wifi connection");

    if(ERROR_CODE){
      uint8_t pos = 37;

      if(bitRead(ERROR_CODE, ERROR_LOW_VOLTAGE)){
        lcd.setCursor(1, pos);
        lcd.print("Low voltage!");
        pos += 9;
      }
      if(bitRead(ERROR_CODE, ERROR_OVERCURRENT)){
        lcd.setCursor(1, pos);
        lcd.print("Overcurrent!");
        pos += 9;
      }
      if(bitRead(ERROR_CODE, ERROR_NO_SD)){
        lcd.setCursor(1, pos);
        lcd.print("No SD card!");
        pos += 9;
      }
    }
}

void SetupScreen(){
    lcd.begin(SCREEN_ADDR);
    lcd.clearDisplay();
}

void UpdateScreen(){
    lcd.clearDisplay();
    lcd.setTextSize(0);
    lcd.setTextColor(1);
    
    if(millis() % 10000 <= 5000) Screen_1();
    else Screen_2();
    
    lcd.display();
}