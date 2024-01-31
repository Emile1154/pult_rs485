#pragma once
#ifndef _MODBUS_CONTROLLER_H_
#define _MODBUS_CONTROLLER_H_


#include <controller/panel_controller.h>
#include <stdint.h>
#include <model/queue.h>
extern "C" {
    #include <form/indicator.h>
}

#define _CHAR_TIMEOUT_ 40
#define TRY_COUNT_MAX  10
class ModbusController : public PanelController{
private:
    uint8_t counter = 0;     
    uint8_t  state = 0;
    uint64_t timer = 0;
    uint8_t readed = 0;
    uint8_t bufferInput[32];

    uint8_t cur_cmd = 0;
    uint8_t try_again_cnt = 0;
        
    inline void pull(){
        uint32_t tm = get_ms();
        counter = 0;     
        while(get_ms() - tm <= _CHAR_TIMEOUT_ && counter < sizeof(bufferInput)){  
            if(Serial.available()){
                tm = get_ms();
                bufferInput[counter] = Serial.read();
                counter++;
            }
        }        
    }
    inline void clearBufferInput(){
        for (uint8_t i = 0; i < sizeof(bufferInput); i++)
        {
            bufferInput[i] = 0; 
        }
        counter = 0;
    }
    inline void init_uart(){
        Serial.begin(9600, SERIAL_8O1);
        while(! Serial);
        clearBufferInput();  //init buf
    }
    inline void init_requests(){
        requests[STOP_ALL] = new request_modbus_write_pack_t(  
            ALL,
            0x00,
            0x00,
            0x00,
            0x00
        );
        requests[START_ALL_CW] = new request_modbus_write_pack_t(
            ALL,
            0x00,
            0x00,
            0x00,
            0x01
        );
        requests[START_ALL_CCW] = new request_modbus_write_pack_t( 
            ALL,
            0x00,
            0x00,
            0x00,
            0x02
        );
        requests[LAYER_START_MAX_CW] = new request_modbus_group_write_pack_t( 
            LAYER_ID,
            0x00,
            0x00,
            0x00,
            0x02
        );
        //init group command, is constant request
        requests[LAYER_START_MAX_CW]->set_value(   1, 0); //set dir  cw
        requests[LAYER_START_MAX_CW]->set_value(5000, 1); //set max speed

        requests[LAYER_START_MAX_CCW] = new request_modbus_group_write_pack_t( 
            LAYER_ID,
            0x00,
            0x00,
            0x00,
            0x02
        );
        //init group command, is constant request
        requests[LAYER_START_MAX_CCW]->set_value(   2, 0); //set dir  ccw
        requests[LAYER_START_MAX_CCW]->set_value(5000, 1); //set max speed

        
        requests[LAYER_STOP_SET_SPEED] = new request_modbus_group_write_pack_t( 
            LAYER_ID,
            0x00,
            0x00,
            0x00,
            0x02
        );

        //init group command, is not constant request
        requests[LAYER_STOP_SET_SPEED]->set_value(   0, 0); //set dir none
        
        requests[LAYER_SET_SPEED] = new request_modbus_write_pack_t( 
            LAYER_ID,
            0x00,
            0x01,
            0x00,
            0x00
        );
        requests[REEL_SET_SPEED] = new request_modbus_write_pack_t(
            REEL_ID,
            0x00,
            0x01,
            0x00,
            0x00
        );

        requests[LAYER_REVERSE] = new request_modbus_write_pack_t(
            LAYER_ID,
            0x00,
            0x00,
            0x00,
            0x00
        );
    }

public:
    inline void init_modbus(){
        init_requests();
        init_uart();
    }          

    void modbus_automate(){
        if(state == 0 && get_ms() - timer >= T240){ //send command t240 interval
            if(! queue.isEmpty()){ 
                cur_cmd = queue.pop();
                requests[cur_cmd]->send();
                state++;
            }
            timer = get_ms();
        }
        if( state == 1 && Serial.available() ){
            pull();// read answer
        }
        if( (state && get_ms() - timer >= TIMEOUT ) || (counter > 0 && state ) ){
            uint8_t status = 4;

            //LED CONTROL
            if(cur_cmd == START_ALL_CW){
                up_btn_led_on();
            }
            if(cur_cmd == START_ALL_CCW){
                down_btn_led_on();
            }
            if(cur_cmd == STOP_ALL){
                led_off();
            }

            if(counter != 0){
               status = requests[cur_cmd]->handle(bufferInput, counter);
            }
            clearBufferInput();
            if(cur_cmd != LAYER_SET_SPEED && cur_cmd != REEL_SET_SPEED && requests[cur_cmd]->id != ALL){
                if(status == 0){                //OK
                    try_again_cnt = 0;
#if DEBUG_ENABLED
                    println("success");
#endif
                }else{                          //STATUS ERROR 
                    // TRY SEND AGAIN 
                    if(try_again_cnt <= TRY_COUNT_MAX){
                        queue.push(cur_cmd);
                        try_again_cnt++;
                    }else{
                        error_view();   // LED UP AND LED DOWN ENABLE
                    }
                    if(status == 1){                
#if DEBUG_ENABLED
                        println("trash");
#endif
                    }
                    if(status == 2){
#if DEBUG_ENABLED
                       println("crc err");
#endif
                    }
                    if(status == 3){
#if DEBUG_ENABLED
                        println("noise");
#endif
                    }
                    if(status == 4){
#if DEBUG_ENABLED
                        println("timeout");
#endif
                    }
                }
            }
            state = 0;
            timer = get_ms();
        }
    }
};
#endif      