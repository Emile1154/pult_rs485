#include <Carotage.h>

Carotage::execute(uint8_t num_key){
    switch (num_key)
    {
    case 1:
        modbus.up();
        break;
    case 2:
        modbus.down();
        break;
    case 3:
        modbus.left();
        break;
    case 4:
        modbus.right();
        break;
    case 5:
        modbus.stop();
        break;
    case 6:
        modbus.layer_stop();
        break;
    case 7:
        modbus.reel_set_speed();
        break;
    case 8:
        modbus.layer_set_speed();
        break;    
    }
}

