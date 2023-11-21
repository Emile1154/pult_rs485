#include <ModbusRTU.h>
#include <data.h>
#include <Panel.h>  //need for give speed values

ModbusRTU::ModbusRTU(const uint16_t speed, const uint8_t serialType, const uint16_t timeout)
{
    Serial.begin(speed, serialType);
    Serial.setTimeout(timeout);
}

ModbusRTU::~ModbusRTU()
{
}

void ModbusRTU::up() const{
    for (uint8_t i = 0; i < sizeof(START_ALL_CW); i++)
    {
        Serial.write(START_ALL_CW[i]);
    }
    Serial.flush();
}

void ModbusRTU::down() const{
    for (uint8_t i = 0; i < sizeof(START_ALL_CCW); i++)
    {
        Serial.write(START_ALL_CCW[i]);
    }
    Serial.flush();
}

void ModbusRTU::left() const{
    for (uint8_t i = 0; i < sizeof(LAYER_START_MAX_CCW); i++)
    {
        Serial.write(LAYER_START_MAX_CCW[i]);
    }
    Serial.flush();
}

void ModbusRTU::right() const{
    for (uint8_t i = 0; i < sizeof(LAYER_START_MAX_CW); i++)
    {
        Serial.write(LAYER_START_MAX_CW[i]);
    }
    Serial.flush();
}

void ModbusRTU::stop() const{
    for (uint8_t i = 0; i < sizeof(STOP_ALL); i++)
    {
        Serial.write(STOP_ALL[i]);
    }
    Serial.flush();
}

void ModbusRTU::layerStop() const{
    updateSpeed(2);  //update array command
    uint8_t size = sizeof(LAYER_STOP_SET_SPEED);
    uint16_t crc16 = getCrc16(LAYER_STOP_SET_SPEED, size - 2);
    LAYER_STOP_SET_SPEED[size - 2] = (crc16 >> 8) & 0xFF; //hi byte
    LAYER_STOP_SET_SPEED[size - 1] =  crc16 & 0xFF; //lo_byte
    for (uint8_t i = 0; i < size; i++)
    {
        Serial.write(LAYER_STOP_SET_SPEED[i]);
    }
    Serial.flush();
}

void ModbusRTU::layerSetSpeed() const{
    updateSpeed(1);  //update array command
    uint8_t size = sizeof(LAYER_SET_SPEED);
    uint16_t crc16 = getCrc16(LAYER_SET_SPEED, size - 2);
    LAYER_SET_SPEED[size - 2] = (crc16 >> 8) & 0xFF; //hi byte
    LAYER_SET_SPEED[size - 1] =  crc16 & 0xFF; //lo_byte
    for (uint8_t i = 0; i < size; i++)
    {
        Serial.write(LAYER_SET_SPEED[i]);
    }
    Serial.flush();
}

void ModbusRTU::reelSetSpeed() const{
    updateSpeed(0);  //update array command
    uint8_t size = sizeof(REEL_SET_SPEED);
    uint16_t crc16 = getCrc16(REEL_SET_SPEED, size - 2);
    REEL_SET_SPEED[size - 2] = (crc16 >> 8) & 0xFF; //hi byte
    REEL_SET_SPEED[size - 1] =  crc16 & 0xFF; //lo_byte
    for (uint8_t i = 0; i < size; i++)
    {
        Serial.write(REEL_SET_SPEED[i]);
    }
    Serial.flush();
}

void ModbusRTU::updateSpeed(uint8_t select){
    uint16_t speed;
    uint8_t hi_byte, lo_byte;
    if(select == 0){ //update reel speed
        speed = Panel::reel_value;
        hi_byte = (speed >> 8) & 0xFF;
        lo_byte =  speed & 0xFF;
        REEL_SET_SPEED[4] = hi_byte;
        REEL_SET_SPEED[5] = lo_byte;
        return;
    }
    speed = Panel::layer_value;
    hi_byte = (speed >> 8) & 0xFF;
    lo_byte =  speed & 0xFF;
    if(select == 1){ //update layer speed
        LAYER_SET_SPEED[4] = hi_byte;
        LAYER_SET_SPEED[5] = lo_byte;
        return;
    }
    //update layer speed in group command
    LAYER_STOP_SET_SPEED[9]  = hi_byte;
    LAYER_STOP_SET_SPEED[10] = lo_byte; 
    return;
}

uint16_t ModbusRTU::getCrc16(uint8_t bufferArray[], uint8_t length){
    unsigned int temp, temp2, flag;
    temp = 0xFFFF;
    for (unsigned char i = 0; i < length; i++) 
    {
        temp = temp ^ bufferArray[i];
        for (unsigned char j = 1; j <= 8; j++) 
        {
            flag = temp & 0x0001;
            temp >>= 1;
            if (flag)   temp ^= 0xA001;
        }
    }
    temp2 = temp >> 8;
    temp = (temp << 8) | temp2;
    temp &= 0xFFFF;
    return temp;
}
