#pragma once
#include <stdint.h>
#include <ModbusRTU.h>
class Carotage{
    private:
        const ModbusRTU modbus;
        uint16_t getCrc16(uint8_t bufferArray[], uint8_t length);
    public:
        Carotage(const ModbusRTU& modbus); //constructor
        ~Carotage();                //destructor
        void up() const;
        void down() const;
        void stop() const;
        void left() const;
        void right() const;
        void layer_stop() const;
        void layer_set_speed() const;
        void reel_set_speed() const;

        //and next realization...
};