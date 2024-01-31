#pragma once
#ifndef _MODBUS_CONTROLLER_H_
#define _MODBUS_CONTROLLER_H_
#define _CHAR_TIMEOUT_ 40

#include <controller/panel_controller.h>

#include <stdint.h>
#include <model/queue.h>

extern "C" {
    
    #include <form/indicator.h>
}



class ModbusController : public PanelController{
private:
    uint8_t counter = 0;     
    uint8_t  state = 0;
    uint64_t timer = 0;
    uint8_t readed = 0;
    uint8_t bufferInput[64];
    

    request_modbus_pack_t * request = nullptr;
    
    inline void pull(){
        uint32_t tm = get_ms();
        counter = 0;     
        while(get_ms() - tm <= _CHAR_TIMEOUT_ && counter < sizeof(bufferInput)){  //testing 03 03 00 08 00 01 04 2A
            if(Serial.available()){
                tm = get_ms();
                bufferInput[counter] = Serial.read();
                counter++;
            }
        }        
    }


    inline void clearBufferInput(){
        for (uint8_t i = 0; i < 64; i++)
        {
            bufferInput[i] = 0; 
        }
    }

    inline void init_uart(){
        Serial.begin(9600, SERIAL_8O1);
        while(! Serial);
    }

    inline void init_requests(){
        requests[0] = new request_modbus_write_pack_t(  //STOP ALL
            ALL,
            0x00,
            0x00,
            0x00,
            0x00
        );
        requests[1] = new request_modbus_write_pack_t( //START ALL CW
            ALL,
            0x00,
            0x00,
            0x00,
            0x01
        );
        requests[2] = new request_modbus_write_pack_t( //START ALL CWW
            ALL,
            0x00,
            0x00,
            0x00,
            0x02
        );
        requests[3] = new request_modbus_group_write_pack_t( //LAYER START MAX CW
            LAYER_ID,
            0x00,
            0x00,
            0x00,
            0x02
        );
        requests[4] = new request_modbus_group_write_pack_t( //LAYER_START_MAX_CCW
            LAYER_ID,
            0x00,
            0x00,
            0x00,
            0x02
        );
        requests[5] = new request_modbus_group_write_pack_t( //LAYER_STOP_SET_SPEED
            LAYER_ID,
            0x00,
            0x00,
            0x00,
            0x02
        );
        requests[6] = new request_modbus_write_pack_t( //LAYER_SET_SPEED
            LAYER_ID,
            0x00,
            0x01,
            0x00,
            0x00
        );
        requests[7] = new request_modbus_write_pack_t( //REEL_SET_SPEED
            REEL_ID,
            0x00,
            0x01,
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
                request = requests[queue.pop()];
                request->send();
                state++;
            }

            timer = get_ms();
        }
        if( state == 1 && Serial.available() ){
            pull();// read command sensors
        }
        if( (state && get_ms() - timer >= TIMEOUT ) || (counter > 0 && state ) ){
            uint8_t status = request->handle( bufferInput, counter);
            state =0;
            timer = get_ms();
#if DEBUG_ENABLED
            println(status);
#endif 

        }
    }


};
#endif      