#include <stdint.h>
#include <Carotage.h>
#include <CommandInterface.h>
#include <Panel.h>
#include <avr/interrupt.h>
//#include <SoftwareSerial.h>
//SoftwareSerial debug(8,10);

// void initDebug(){
//   debug.begin(9600);
// }

void initTimer(void){
    TCCR1B |= (1 << WGM12);
    // Set the prescaler to 64 (16MHz / 64 = 250kHz)
    TCCR1B |= (1 << CS11) | (1 << CS10);
    // Calculate the compare match value for 1ms
    OCR1A = 249;
    // Enable the compare match interrupt
    TIMSK1 |= (1 << OCIE1A);
    // Enable global interrupts
    sei();
}

int main(){
  //initDebug();
  initTimer();
  ModbusRTU modbus(9600, SERIAL_8O1, 100);
  Carotage carotage(modbus); //maybe add old values from EEPROM in constructor in future
  Panel panel(  
      &carotage`  
      //&modbus         
  );
  panel.initPins();
  panel.initADC();
  while(1){
    panel.checkKeyboard();
    panel.checkPots();
    panel.query();
  }
  return 0;
}
