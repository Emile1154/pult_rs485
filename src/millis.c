#include <millis.h>

ISR(TIMER1_COMPA_vect){
    t_millis++;
}

uint64_t millis(){
    uint64_t result;
    // Ensure this cannot be disrupted
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        result = t_millis;
    }
    return result;
}