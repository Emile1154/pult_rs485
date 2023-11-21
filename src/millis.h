#ifndef _MILLIS_H_
#define _MILLIS_H_

#include <stdint.h>
volatile uint64_t t_millis;
ISR(TIMER1_COMPA_vect);
void initTimer();
uint64_t millis();
#endif