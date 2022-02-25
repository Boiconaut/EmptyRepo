#ifndef MOTO_H
#define MOTO_H

#include "common.h"

#define LOAD_CURRENT 5.00

#define MIN_CELL 0
#define HOUR_LOW_CELL 1
#define HOUR_HIGH_CELL 2

extern TimeSpan moto_time; 
extern DateTime moto_relative;
extern uint16_t motohour;

extern uint8_t min_cell; 
extern uint8_t hour_low_cell; 
extern uint8_t hour_high_cell; 

extern float loadCurrent;
void read();

void SetupMotohours();
void Reset();
void Tick();
void SaveMoto();

#endif