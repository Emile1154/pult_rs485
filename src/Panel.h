#pragma once
#include <CommandInterface.h>
#include <settings.h>
#include <Queue.h>
#include <avr/io.h>
#include <Carotage.h>

class Panel
{
private:
    Carotage & carotage;
    uint16_t filter(uint8_t _pin, uint16_t component, uint16_t value);
    uint16_t filter(uint8_t _pin, uint16_t component);
    uint16_t readADC(uint8_t _pin);
    
public:
    static uint16_t reel_value;
    static uint16_t layer_value;
    void initPins(); 
    void initADC();
    void checkKeyboard();
    void checkPots(); 
    void query();
    Panel(
        Carotage & carotage
    ):carotage(carotage);
    ~Panel();
};
