#pragma once
#ifndef _DEBUG_CONTROLLER_H_
#define _DEBUG_CONTROLLER_H_

#if DEBUG_ENABLED

#include <SoftwareSerial.h>
SoftwareSerial debug(8,10);

void run_debug() {
    debug.begin(9600);    
}

void println(const char * str) {
    debug.println(str);
}

void print(const char * str) {
    debug.print(str);
}

void print(uint8_t val) {
    debug.print(val);
}

void println(uint16_t val) {
    debug.println(val);
}



#endif



#endif