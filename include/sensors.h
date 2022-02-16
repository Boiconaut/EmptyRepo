#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include "clock.h"

#define VoltPin 36            
#define DischargeCurrPin 39   
#define ChargeCurrPin 34   

#define SENSOR_UPDATE_PERIOD 100

#define STATE_REST 0       
#define STATE_CHARGE 1      
#define STATE_DISCHARGE 2 

#define VMAX 3.4     
#define V90 3.23      
#define V80 3.22
#define V70 3.21
#define V60 3.19
#define V50 3.18
#define V40 3.16
#define V30 3.13
#define V20 3.08
#define V10 2.96
#define VMIN 2.20

#define kVD 22
#define kI 30.3
#define kVSOC_INIT 4.4

class SensorsHandler{
private:
    float _capacity;
    uint8_t STATE_BEFORE;
    uint8_t NOM_VOLTAGE;
    uint8_t NOM_CAPACITY;
        
    float kVSOC;   //For calculating SOC: kVD / 5 for 12 V  

    uint16_t VoltageValue; 
    uint16_t ChargeCurrentValue; 
    uint16_t DischargeCurrentValue; 
    uint16_t TemperatureValue; 

    float moment_voltage; 
    float moment_current; 
    float moment_power; 
    uint8_t moment_SOC; 

    float voltage; 
    float current; 
    float power; 
    uint8_t SOC; 
    float temperature; 

    float voltages[1000 / SENSOR_UPDATE_PERIOD]; 
    float currents[1000 / SENSOR_UPDATE_PERIOD]; 
    float powers[1000 / SENSOR_UPDATE_PERIOD]; 
    float temperatures[1000 / SENSOR_UPDATE_PERIOD]; 
    uint8_t socs[1000 / SENSOR_UPDATE_PERIOD]; 

    float minute_currents[60]; 
    float average_current; 

    uint8_t counter; 
    uint8_t minute_current_counter;

    TimeSpan state_time;
    DateTime change_state_time;

    void setupAnalogInputs();
    uint8_t getSOC(float v);
    void tick(ClockHandler *clk);
public:
    uint8_t STATE;
    float vmax, v90, v80, v70, v60, v50, v40, v30, v20, v10, vmin;

    SensorsHandler();
    ~SensorsHandler();

    void ReadSensors();
    void GetSecondAverage(ClockHandler *clk);
    void GetMinuteAverageCurrent();
    void UpdateVoltageMap();

    uint8_t GetSOC();
    float GetVoltage();
    float GetCurrent();
    float GetPower();
    float GetCapacity();

    uint8_t GetNomCapacity();
    uint8_t GetNomVoltage();
    void SetNomVoltage(uint8_t v);
    void SetNomCapacity(uint8_t c);
    float GetAverageCurrent();
    void SetVoltageQuot(float k);
    void SetChangeStateTime(DateTime *dt);

    uint8_t GetMomentSOC();
    float GetMomentVoltage();
    float GetMomentCurrent();
    float GetMomentPower();

    uint16_t GetHours();
    uint8_t GetMinutes();
    uint8_t GetSeconds();
};

#endif