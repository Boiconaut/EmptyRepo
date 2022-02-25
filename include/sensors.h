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

#define k_VD 22
#define k_I 30.3
#define kVSOC_INIT 4.4

extern float _capacity;
extern uint8_t STATE_BEFORE;
extern uint8_t NOM_VOLTAGE;
extern uint8_t NOM_CAPACITY; 
extern uint16_t state_hour;
    
extern float kVSOC;   //For calculating SOC: kVD / 5 for 12 V  
extern float kVD;
extern float kI;

extern uint16_t VoltageValue; 
extern uint16_t ChargeCurrentValue; 
extern uint16_t DischargeCurrentValue; 
extern uint16_t TemperatureValue; 

extern float moment_voltage; 
extern float moment_current; 
extern float moment_power; 
extern uint8_t moment_SOC; 

extern float voltage; 
extern float current; 
extern float power; 
extern uint8_t SOC; 
extern float temperature; 

extern float voltages[1000 / SENSOR_UPDATE_PERIOD]; 
extern float currents[1000 / SENSOR_UPDATE_PERIOD]; 
extern float powers[1000 / SENSOR_UPDATE_PERIOD]; 
extern float temperatures[1000 / SENSOR_UPDATE_PERIOD]; 
extern uint8_t socs[1000 / SENSOR_UPDATE_PERIOD]; 

extern float minute_currents[60]; 
extern float average_current; 

extern uint8_t counter; 
extern uint8_t minute_current_counter;

extern TimeSpan state_time;
extern DateTime change_state_time;

void setupAnalogInputs();
uint8_t getSOC(float v);
void tick();

extern uint8_t STATE;
extern float vmax, v90, v80, v70, v60, v50, v40, v30, v20, v10, vmin;

void SetupSensors();
void ReadSensors();
void GetSecondAverage();
void GetMinuteAverageCurrent();
void UpdateVoltageMap();

#endif