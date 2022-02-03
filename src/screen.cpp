#include <WiFi.h>
#include "screen.h"

Screen::Screen(uint8_t SDA, uint8_t SCL) : Adafruit_SH1106(SDA, SCL){
  
}

void Screen::Screen_1(ClockHandler *clk, SensorsHandler *sensors){
    setCursor(1, 1);
    if(clk->GetDay() < 10) this->print("0");
    this->print(clk->GetDay());
    this->print("-");
    if(clk->GetMonth() < 10) this->print("0");
    this->print(clk->GetMonth());
    this->print("-");
    this->print(clk->GetYear());
    this->print(" ");
    if(clk->GetHours() < 10) this->print("0");
    this->print(clk->GetHours());
    this->print(":");
    if(clk->GetMinutes() < 10) this->print("0");
    this->print(clk->GetMinutes());
    this->print(":");
    if(clk->GetSeconds() < 10) this->print("0");
    this->print(clk->GetSeconds());

    setCursor(1, 10);
    this->print("V=");
    this->print(sensors->GetVoltage());
    this->print(" V");

    setCursor(1, 19);
    this->print("I=");
    this->print(sensors->GetCurrent());
    this->print(" A");

    setCursor(1, 28);
    this->print("P=");
    this->print(sensors->GetPower());
    this->print(" W");

    uint8_t level = sensors->GetSOC() / 6;
    for(uint8_t i = 0; i < level; i++){
      fillRect(i * 8, 37, 7, 23, 1);
    }
}

void Screen::Screen_2(ErrorHandler *error, SensorsHandler *sensors){
    setCursor(1, 1);
    this->print("Nominal: ");
    this->print(sensors->GetNomVoltage());
    this->print(" V  ");
    this->print(sensors->GetNomCapacity());
    this->print(" Ah");

    setCursor(1, 10);
    if(sensors->STATE == 1) this->print("On charge: ");
    else if(sensors->STATE == 2) this->print("On discharge: ");
    else this->print("Relaxing: ");
    String h = sensors->GetHours() < 0 ? "0" + String(sensors->GetHours()) : String(sensors->GetHours());
    String m = sensors->GetMinutes() < 0 ? "0" + String(sensors->GetMinutes()) : String(sensors->GetMinutes());
    String s = sensors->GetSeconds() < 0 ? "0" + String(sensors->GetSeconds()) : String(sensors->GetSeconds());
    String state_time = h + ":" + m + ":" + s;
    this->print(state_time);

    setCursor(1, 19);
    this->print("Coulomb: ");
    this->print(sensors->GetCapacity());
    this->print(" mAh");

    setCursor(1, 28);
    WiFi.status() == WL_CONNECTED ? this->print("Wifi connected") : this->print("No Wifi connection");

    if(error->ERROR_CODE){
      uint8_t pos = 37;

      if(bitRead(error->ERROR_CODE, ERROR_LOW_VOLTAGE)){
        setCursor(1, pos);
        this->print("Low voltage!");
        pos += 9;
      }
      if(bitRead(error->ERROR_CODE, ERROR_OVERCURRENT)){
        setCursor(1, pos);
        this->print("Overcurrent!");
        pos += 9;
      }
      if(bitRead(error->ERROR_CODE, ERROR_NO_SD)){
        setCursor(1, pos);
        this->print("No SD card!");
        pos += 9;
      }
    }
}

void Screen::SetupScreen(){
    this->begin(SCREEN_ADDR);
    clearDisplay();
}

void Screen::UpdateScreen(ClockHandler *clk, SensorsHandler *sensors, ErrorHandler *error){
    clearDisplay();
    setTextSize(0);
    setTextColor(1);
    
    if(millis() % 10000 <= 5000) Screen_1(clk, sensors);
    else Screen_2(error, sensors);
    
    display();
}