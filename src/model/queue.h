#pragma once
#ifndef QUEUE_H_
#define QUEUE_H_
#include <config/settings.h>
#include <stdint.h>




uint8_t isfull();
uint8_t isempty();
void push(uint8_t newitem);
uint8_t pop();

#endif