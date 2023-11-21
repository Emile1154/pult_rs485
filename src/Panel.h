#pragma once
#include <CommandInterface.h>
#include <settings.h>
#include <Queue.h>
#include <avr/io.h>

class Panel
{
private:
    /* data */
    //uint32_t timer;
    const CommandInterface * upCommand;
    const CommandInterface * downCommand;
    const CommandInterface * leftCommand;
    const CommandInterface * rightCommand;
    const CommandInterface * stopCommand;
    const CommandInterface * layerStopCommand;
    const CommandInterface * reelSetSpeedCommand;
    const CommandInterface * layerSetSpeedCommand;
    Queue<const CommandInterface*> queue = Queue<const CommandInterface*>(64); 
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
        const CommandInterface * upCommand,
        const CommandInterface * downCommand,
        const CommandInterface * leftCommand,
        const CommandInterface * rightCommand,
        const CommandInterface * stopCommand,
        const CommandInterface * layerStopCommand,
        const CommandInterface * reelSetSpeedCommand,
        const CommandInterface * layerSetSpeedCommand
        //encoder, wifi, bluetooth and next objects
    );
    ~Panel();
};
