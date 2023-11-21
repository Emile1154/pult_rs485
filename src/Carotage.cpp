#include <Carotage.h>

Carotage::Carotage(const ModbusRTU& modbus) : modbus(modbus){

}

Carotage::~Carotage(){

}

void Carotage::up() const{
    modbus.up();
}

void Carotage::down() const{
    modbus.down();
}

void Carotage::left() const{
    modbus.left();
}

void Carotage::right() const{
    modbus.right();
}

void Carotage::stop() const{
    modbus.stop();
}

void Carotage::layer_stop() const{
    modbus.layerStop();
}

void Carotage::layer_set_speed() const{
    modbus.layerSetSpeed();
}

void Carotage::reel_set_speed() const{
    modbus.reelSetSpeed();
}
//next realisation