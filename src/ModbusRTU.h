#pragma once
#include <HardwareSerial.h>


class ModbusRTU
{
private:
    /* data */
    uint16_t getCrc16(uint8_t bufferArray[], uint8_t length);
    void updateSpeed(uint8_t select);
public:
    void up() const;
    void down() const;
    void left() const;
    void right() const;
    void stop() const;
    void layerStop() const;
    void layerSetSpeed() const;
    void reelSetSpeed() const;
    ModbusRTU(/* args */const uint16_t speed, const uint8_t serialType, const uint16_t timeout);
    ~ModbusRTU();
};


