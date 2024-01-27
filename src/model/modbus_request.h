#pragma once
#ifndef _MODBUS_REQUEST_H_
#define _MODBUS_REQUEST_H_

#include <stdint.h>
#include <stdlib.h>
#include <HardwareSerial.h>

/* 
 * id
 * function               0x06
 * ADDRESS                HIGH
 * ADDRESS                LOW
 * VALUE                  HIGH
 * VALUE                  LOW
 * CRC                    HIGH
 * CRC                    LOW
*/
typedef struct{
    uint8_t id;
    uint8_t fx;

    uint8_t address_hi;
    uint8_t address_lo;

    uint8_t crc_hi;
    uint8_t crc_lo;

    virtual uint8_t handle(uint8_t *answer, uint8_t len) = 0;
    virtual void update_crc16() = 0;
    virtual void set_value(uint16_t value, uint8_t reg) = 0;
    virtual void send() = 0;

    
uint16_t get_crc16(uint8_t * buffer, uint8_t len){
    unsigned int temp, temp2, flag;
    temp = 0xFFFF;
    for (unsigned char i = 0; i < len; i++) 
    {
        temp = temp ^ buffer[i];
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

} request_modbus_pack_t;


typedef struct request_modbus_write_pack_t : request_modbus_pack_t
{
    uint8_t value_hi;
    uint8_t value_lo;

    void set_value(uint16_t value, uint8_t reg) override{
        value_hi = (value >> 8) & 0xFF;
        value_lo = value & 0xFF;

        update_crc16();
    }

    uint8_t handle(uint8_t *answer, uint8_t len) override{
        if(len < 4 || len > 8 || answer[0] != id){
            return 1;
        }
        if(crc_hi != answer[len-2] || crc_lo != answer[len-1]){
            return 2;
        }    
        return 0;
    }

    void send() override{
        Serial.write(id);
        Serial.write(fx);
        Serial.write(address_hi);
        Serial.write(address_lo);
        Serial.write(value_hi);
        Serial.write(value_lo);
        Serial.write(crc_hi);
        Serial.write(crc_lo);

        Serial.flush();
    }

    void update_crc16() override{
        uint8_t * temp = (uint8_t*) malloc(6 * sizeof(uint8_t));
        temp[0] = id;
        temp[1] = fx;
        temp[2] = address_hi;
        temp[3] = address_lo;
        temp[4] = value_hi;
        temp[5] = value_lo;

        uint16_t crc = get_crc16(temp, 6);

        this->crc_hi = (crc >> 8) & 0xFF;
        this->crc_lo = crc & 0xFF;

        free(temp);
        temp = nullptr;
    }

    request_modbus_write_pack_t(
        uint8_t id, 
        uint8_t address_hi, 
        uint8_t address_lo, 
        uint8_t value_hi, 
        uint8_t value_lo
    )
    {   
        this->id = id; 
        this->fx = 0x06;
        this->address_hi = address_hi;
        this->address_lo = address_lo;
        this->value_hi   = value_hi;
        this->value_lo   = value_lo; 

        update_crc16();
    };

} request_modbus_write_pack_t;

/* 
 * id
 * function               0x10
 * START_reg              HIGH
 * START_reg              LOW
 * COUNT_reg              HIGH
 * COUNT_reg              LOW
 * COUNT DATA BYTES       EQUALS 2*COUNT_reg
 * FIRST_GROUP_DATA       HIGH
 * FIRST_GROUP_DATA       LOW
 * SECOND_GROUP_DATA      HIGH
 * SECOND_GROUP_DATA      LOW
 * CRC                    HIGH
 * CRC                    LOW
*/
typedef struct request_modbus_group_write_pack_t : request_modbus_pack_t
{   
    uint8_t *temp;

    uint8_t cnt_reg_hi;
    uint8_t cnt_reg_lo;
    uint8_t cnt_bytes; 
    uint8_t *data;

    void set_value(uint16_t value, uint8_t reg) override{
        uint8_t index_hi = reg * 2; //high reg for argument
        if(index_hi > cnt_bytes){
            return;
        }
        uint8_t index_lo = index_hi + 1;

        this->data[index_hi] = (value >> 8) & 0xFF;
        this->data[index_lo] = value & 0xFF;

        update_crc16();
    }

    void send() override{
        Serial.write(id);
        Serial.write(fx);
        Serial.write(cnt_reg_hi);
        Serial.write(cnt_reg_lo);
        Serial.write(cnt_bytes);
        for(uint8_t i = 0; i < cnt_bytes; i++){
            Serial.write(data[i]);
        }
        Serial.write(crc_hi);
        Serial.write(crc_lo);

        Serial.flush();
    }

    uint8_t handle(uint8_t *answer, uint8_t len) override{
        if(len < 3){
            return 1;
        }
        uint16_t crc = (answer[len-2] << 8) | answer[len-1];
        if(crc != get_crc16(answer, len-2)){
            return 2;
        }
        for (uint8_t i = 0; i < 5; i++)
        {
            if(answer[i] != temp[i]){
                return 3;
            }
        }
        return 0;
    }

    void update_crc16() override{
        //copy data to temp
        for(uint8_t i = 0; i < cnt_bytes; i++){
            temp[i+7] = data[i];
        }
        uint16_t crc16 = get_crc16(temp, 7+cnt_bytes);
        this->crc_hi = (crc16 >> 8) & 0xFF;
        this->crc_lo = (crc16 & 0xFF);
    }

    request_modbus_group_write_pack_t
    (
        uint8_t id, 
        uint8_t address_hi, 
        uint8_t address_lo, 
        uint8_t cnt_reg_hi, 
        uint8_t cnt_reg_lo
    )
    {
        this->id = id;
        this->fx = 0x10;
        this->address_hi = address_hi;
        this->address_lo = address_lo;
        this->cnt_reg_hi = cnt_reg_hi;
        this->cnt_reg_lo = cnt_reg_lo;
        this->cnt_bytes = ((cnt_reg_hi << 8) | cnt_reg_lo) * 2;
        this->data = (uint8_t*) malloc(this->cnt_bytes * (sizeof(uint8_t)));


        //for update crc
        this->temp = (uint8_t*) malloc((7+this->cnt_bytes) * sizeof(uint8_t));
        //init temp
        this->temp[0] = this->id;
        this->temp[1] = this->fx;
        this->temp[2] = this->address_hi;
        this->temp[3] = this->address_lo;
        this->temp[4] = this->cnt_reg_hi;
        this->temp[5] = this->cnt_reg_lo;
        this->temp[6] = this->cnt_bytes;
    }
    ~request_modbus_group_write_pack_t(){
        free(this->data);
        this->data = nullptr;
        free(this->temp);
        this->temp = nullptr;
    }

} request_modbus_group_write_pack_t;


/* 
 * id
 * function               0x03
 * ADDRESS                HIGH
 * ADDRESS                LOW
 * COUNT_reg              HIGH
 * COUNT_reg              LOW
 * CRC                    HIGH
 * CRC                    LOW
*/
typedef struct request_modbus_read_pack_t : request_modbus_pack_t
{
    uint8_t cnt_reg_hi;
    uint8_t cnt_reg_lo;

    //constructor
    request_modbus_read_pack_t(
        uint8_t id,
        uint8_t address_hi,
        uint8_t address_lo,
        uint8_t cnt_reg_hi,
        uint8_t cnt_reg_lo
    )
    {   
        this->id = id;
        this->fx = 0x03;
        this->address_hi = address_hi;
        this->address_lo = address_lo;
        this->cnt_reg_hi = cnt_reg_hi;
        this->cnt_reg_lo = cnt_reg_lo;

        update_crc16();
    }

    void send() override{
        Serial.write(id);
        Serial.write(fx);
        Serial.write(address_hi);
        Serial.write(address_lo);
        Serial.write(cnt_reg_hi);
        Serial.write(cnt_reg_lo);
        Serial.write(crc_hi);
        Serial.write(crc_lo);

        Serial.flush();
    }
    

    uint8_t handle(uint8_t *answer, uint8_t len) override{
        return 0;
        // if(len < 3 || answer[0] != data[0]){
        //     return 1;
        // }
        // uint16_t crc = (answer[len-2] << 8) | answer[len-1];
        // if(crc != get_crc16(answer, len-2)){
        //     return 2;
        // }
        // uint8_t bytes = answer[2];
        // uint16_t buffer[bytes/2];
        // uint8_t counter = 0;
        // for(uint8_t i = 3; i < (3 + bytes); i+=2){
        //     uint16_t temp = (answer[i] << 8) | answer[i+1];
        //     buffer[counter] = temp;
        //     counter++;
        // }
        // uint16_t startAddress = (data[2] << 8) | data[3];
        // uint8_t key;
        // uint8_t endKey;
        // for(key = 0; key < sizeof(commands); key++){
        //     if(commands[key].address == startAddress){
        //         uint8_t keyTemp;
        //         endKey = key+counter;
        //         uint8_t j = 0;
        //         for(keyTemp = key; keyTemp < endKey; keyTemp++){
        //             commands[keyTemp].value = buffer[j];
        //             j++;
        //         }
        //         break;
        //     }
        // }
        // while(key < endKey){
        //     switch (key)
        //     {
        //         case Address::ID:

        //             break;
        //         case Address::BODRATE:
                
        //             break;
        //         case Address::EVEN:
                
        //             break;
        //         case Address::INTERVAL:
                
        //             break;
        //         case Address::STOP_BIT:
                
        //             break;
        //         case Address::ZERO_TENSION:
                
        //             break;
        //         case Address::CALIB_TENSION:
        //             modbus.getTension().setCorrection(commands[CALIB_TENSION].value);
        //             break;
        //         case Address::ENCODER_WIRE:
        //             if((commands[ENCODER_WIRE].value - 1) == TWI){
        //                 modbus.getEncoder().setInterface(TWI);
                        
        //             }
        //             if((commands[ENCODER_WIRE].value - 1) == SSI){
        //                 modbus.getEncoder().setInterface(SSI);
                        
        //             }
        //             if((commands[ENCODER_WIRE].value - 1) == AB){
        //                 modbus.getEncoder().setInterface(AB);
                        
        //             }
        //             if((commands[ENCODER_WIRE].value - 1) == ABZ){
        //                 modbus.getEncoder().setInterface(ABZ);
        //             }
        //             break;
        //         case Address::INVERT_ENCODER:
        //             modbus.getEncoder().setInvert(commands[INVERT_ENCODER].value);
        //             break;
        //         case Address::CALIB_HALL:
        //             modbus.getMagnet().setPWM(commands[CALIB_HALL].value);
        //             break;
        //         case Address::GET_TENSION:
        //             modbus.getTension().setTension(commands[GET_TENSION].value);
        //             break;
        //         case Address::GET_ENCODER_HI:
                    
        //             break;
        //         case Address::GET_ENCODER_LO:
        //             modbus.getEncoder().updateDepth((commands[GET_ENCODER_HI].value << 16) | commands[GET_ENCODER_LO].value);
        //             break;
        //         case Address::GET_HALL:
        //             modbus.getMagnet().setValue(commands[GET_HALL].value);
        //             break;
        //     }
        //     key++;
        // }
        // return 0;
    }

    void update_crc16() override{
        return;
    }
    void set_value(uint16_t value, uint8_t reg) override{
        return;
    }

} request_modbus_read_pack_t;

#endif