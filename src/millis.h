#ifndef _MILLIS_H_
#define _MILLIS_H_

#include <stdint.h>
volatile uint64_t t__micros;
ISR(TIMER1_COMPA_vect);
void initTimer();
uint64_t _micros();
#endif