#include <controller/panel_controller.h>
#include <controller/modbus_controller.h>


int main() {
    //PanelController panel;
    ModbusController panel;
    panel.init_panel();
    panel.init_modbus();
    while(1){
        panel.read_keypad();
        panel.read_potentiometer();
        panel.modbus_automate();
    }
}