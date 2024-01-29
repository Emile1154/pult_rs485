#pragma once
#ifndef _MODBUS_CONTROLLER_H_
#define _MODBUS_CONTROLLER_H_


#include <controller/panel_controller.h>
#include <model/modbus_request.h>
#include <stdint.h>

extern "C" {
    
    #include <form/indicator.h>
}

#define MAX_REQUESTS 24

class ModbusController : public PanelController{
private:
    uint8_t  state = 0;
    uint64_t timer = 0;
    uint8_t readed = 0;
    request_modbus_pack_t * requests[MAX_REQUESTS];
    request_modbus_pack_t * request = nullptr;
    
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
    }

public:
    inline void init_modbus(){
        init_requests();
        init_uart();
    }          

    void modbus_automate(){
        if(state == 0 && get_ms() - timer >= T240){ //send command t240 interval
        
            if(queue.isEmpty()){ 
                // read command sensors

                return; 
            }

            request = requests[queue.pop()];
        }
        if(state){
            //start reading
        }
        if( (state && get_ms - timer >= TIMEOUT )|| readed){
            //stop reading
        }
    }

};
#endif      