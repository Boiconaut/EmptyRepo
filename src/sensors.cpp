#include "sensors.h"

float kVSOC = kVSOC_INIT;
float kVD = k_VD;
float kI = k_I;
float _capacity = 0.00;
uint8_t STATE = 0;
uint8_t STATE_BEFORE = 0;
uint8_t NOM_VOLTAGE = 12;
uint8_t NOM_CAPACITY = 6;
uint8_t counter = 0;
uint8_t minute_current_counter = 0;
uint16_t state_hour = 0;

TimeSpan state_time = TimeSpan(0, 0, 0, 0);
DateTime change_state_time;

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

float vmax, v90, v80, v70, v60, v50, v40, v30, v20, v10, vmin;
uint16_t millisec = 0;

void SetupSensors(){
    UpdateVoltageMap();
    ReadSensors();
}

void setupAnalogInputs(){
    VoltageValue = analogRead(VoltPin);
    ChargeCurrentValue = analogRead(ChargeCurrPin);                                  
    DischargeCurrentValue = analogRead(DischargeCurrPin); 

    if(VoltageValue < 115) VoltageValue = 0;

    if (ChargeCurrentValue > DischargeCurrentValue) STATE = STATE_CHARGE;
    else if (ChargeCurrentValue < DischargeCurrentValue) STATE = STATE_DISCHARGE;
    else STATE = STATE_REST;

    moment_voltage = VoltageValue * (3.3 / 4095.0) * kVD;   
    moment_current = STATE == STATE_CHARGE ? (ChargeCurrentValue * (3.3 / 4095.0) * kI)
                        : (DischargeCurrentValue * (3.3 / 4095.0) * kI);     
}

uint8_t getSOC(float v){
    if(STATE == STATE_REST){
        if(v >= v90 && v <= vmax){
            return 90 + (v - v90)/(vmax - v90) * 10;
        }
        else if(v >= v80 && v <= v90){
            return 80 + (v - v80)/(v90 - v80) * 10;
        }
        else if(v >= v70 && v <= v80){
            return 70 + (v - v70)/(v80 - v70) * 10;
        }
        else if(v >= v60 && v <= v70){
            return 60 + (v - v60)/(v70 - v60) * 10;
        }
        else if(v >= v50 && v <= v60){
            return 50 + (v - v50)/(v60 - v50) * 10;
        }
        else if(v >= v40 && v <= v50){
            return 40 + (v - v40)/(v50 - v40) * 10;
        }
        else if(v >= v30 && v <= v40){
            return 30 + (v - v30)/(v40 - v30) * 10;
        }
        else if(v >= v20 && v <= v30){
            return 20 + (v - v20)/(v30 - v20) * 10;
        }
        else if(v >= v10 && v <= v20){
            return 10 + (v - v10)/(v20 - v10) * 10;
        }
        else if(v >= vmin && v <= v10){
            return (v - vmin)/(v10 - vmin) * 10;
        }
        else if(v >= vmax){
            return 100;
        }
        else{
            return 0;
        }
    }

    uint8_t prev_soc = 0;

    if(STATE == STATE_CHARGE){
        return (_capacity / (1000 * NOM_CAPACITY)) * 100;
    }

    if(STATE == STATE_DISCHARGE){
        return 100 - (_capacity / (1000 * NOM_CAPACITY)) * 100;
    }
}

void tick(){
    state_time = rtc.now() - change_state_time;
    state_hour = state_time.days() * 24 + state_time.hours();
  
    if(STATE == STATE_DISCHARGE || STATE == STATE_CHARGE){
       _capacity = state_time.totalseconds() * (current / 3.60);
    }
    else _capacity = 0.00;

    if(STATE != STATE_BEFORE) {
        change_state_time = rtc.now();
        state_hour = 0;
    }
    STATE_BEFORE = STATE;
}

void ReadSensors(){
    setupAnalogInputs();
    
    if(STATE == STATE_DISCHARGE){
      moment_current = 0 - moment_current;
    }
    moment_power = moment_voltage * moment_current;
    moment_SOC = getSOC(moment_voltage);

    voltages[counter] = moment_voltage;
    currents[counter] = moment_current;
    powers[counter] = moment_power;
    socs[counter] = moment_SOC;

    counter++;
}

void GetSecondAverage(){
    float sum_voltage = 0;
    float sum_current = 0;
    float sum_power = 0;
    int sum_soc = 0;

    for(int i = 0; i < counter; i++){
      sum_voltage += voltages[i];
      sum_current += currents[i];
      sum_power += powers[i];
      sum_soc += socs[i];
    }

    if(counter > 0){
        voltage = sum_voltage / counter;
        current = sum_current / counter;
        power = sum_power / counter;
        SOC = sum_soc / counter;
    }

    counter = 0;

    if(voltage <= 14.6) NOM_VOLTAGE = 12;
    else if(voltage > 14.6 && voltage <= 29.2) NOM_VOLTAGE = 24;
    else if(voltage > 29.2 && voltage <= 43.8) NOM_VOLTAGE = 36;
    else NOM_VOLTAGE = 48;

    UpdateVoltageMap();
    tick();
    
    minute_currents[minute_current_counter] = current;
    minute_current_counter++;
}

void GetMinuteAverageCurrent(){
    float sum = 0;
    for(int i = 0; i < minute_current_counter; i++){
        sum += minute_currents[i];
    }

    if(minute_current_counter > 0){
        average_current = sum / minute_current_counter;
    }
    minute_current_counter = 0;
}

void UpdateVoltageMap(){
    vmax = VMAX * kVSOC;      
    v90 = V90 * kVSOC;     
    v80 = V80 * kVSOC;
    v70 = V70 * kVSOC;
    v60 = V60 * kVSOC;
    v50 = V50 * kVSOC;
    v40 = V40 * kVSOC;
    v30 = V30 * kVSOC;
    v20 = V20 * kVSOC;
    v10 = V10 * kVSOC;
    vmin = VMIN * kVSOC;
}