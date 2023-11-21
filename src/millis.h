#ifndef _MILLIS_H_
#define _MILLIS_H_

#include <stdint.h>
volatile uint32_t t_millis;
ISR(TIMER1_COMPA_vect);
void initTimer();
uint32_t millis();
#endif