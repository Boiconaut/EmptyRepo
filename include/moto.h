#ifndef MOTO_H
#define MOTO_H

#define LOAD_CURRENT 5.00

#define MIN_CELL 0
#define HOUR_LOW_CELL 1
#define HOUR_HIGH_CELL 2

class MotoHandler{
private:
    uint16_t motohour;
    uint8_t motomin;
    uint8_t motosec;

    uint8_t min_cell; 
    uint8_t hour_low_cell; 
    uint8_t hour_high_cell; 

    float loadCurrent;
    void read();
public:
    MotoHandler();
    ~MotoHandler();
    void SetupMotohours();
    void Reset();
    void Tick();
    void Save();

    uint16_t GetHours();
    uint8_t GetMinutes();
    uint8_t GetSeconds();
};

#endif