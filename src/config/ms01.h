#pragma once
#ifndef _MILLIS_H_
#define _MILLIS_H_
#include <stdint.h>
#include <util/atomic.h>
#include <avr/interrupt.h>
#include <avr/io.h>

ISR(TIMER1_COMPA_vect);
void init_timer();
uint64_t get_ms();

#endif