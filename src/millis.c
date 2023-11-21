#include <millis.h>

ISR(TIMER1_COMPA_vect){
    t_millis++;
}

uint32_t millis(){
    return t_millis;
}