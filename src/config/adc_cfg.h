#pragma once
#ifndef _ADC_CFG_H_
#define _ADC_CFG_H_
#include <avr/io.h>
#include <stdint.h>

void     init_adc();
uint16_t adc_read(uint8_t channel);

#endif