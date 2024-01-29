#include <controller/panel_controller.h>
#include <controller/modbus_controller.h>


int main() {
    PanelController panel;
    ModbusController modbus;
    panel.init_panel();
    while(1){
    panel.read_keypad();
    panel.read_potentiometer();
   // modbus.init_modbus();
}
}