#include <EEPROM.h>
#include "moto.h"

MotoHandler::~MotoHandler(){

}

MotoHandler::MotoHandler(){
    loadCurrent = LOAD_CURRENT;
    min_cell = MIN_CELL; 
    hour_low_cell = HOUR_LOW_CELL; 
    hour_high_cell = HOUR_HIGH_CELL; 
}

void MotoHandler::read(){
    uint8_t motomin = EEPROM.read(min_cell);
    uint8_t low = EEPROM.read(hour_low_cell);
    uint8_t high = EEPROM.read(hour_high_cell);
    motohour = word(high, low);
    uint8_t _day = motohour / 24;
    uint8_t _hour = motohour % 24;
    moto_time = TimeSpan(_day, _hour, motomin, 0);
    moto_relative = *getClock()->GetTimeNow();
}

void MotoHandler::SetupMotohours(){
    EEPROM.begin(512);
    read();
}

void MotoHandler::Reset(){
    EEPROM.write(min_cell, 0);
    EEPROM.write(hour_low_cell, 0);
    EEPROM.write(hour_high_cell, 0);
    EEPROM.commit();
}

void MotoHandler::Tick(){
    moto_time = *getClock()->GetTimeNow() - moto_relative;
    moto_relative = *getClock()->GetTimeNow();
}

void MotoHandler::Save(){
    motohour = 24 * moto_time.days() + moto_time.hours();
    uint8_t high = highByte(motohour);
    uint8_t low = lowByte(motohour);
    EEPROM.put(min_cell, moto_time.minutes());
    EEPROM.put(hour_low_cell, low);
    EEPROM.put(hour_high_cell, high);
    EEPROM.commit();
}

uint16_t MotoHandler::GetHours(){
    return motohour;
}

uint8_t MotoHandler::GetMinutes(){
    return moto_time.minutes();
}

uint8_t MotoHandler::GetSeconds(){
    return moto_time.seconds();
}