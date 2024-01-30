#pragma once
#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <avr/io.h>

#define DEBUG_ENABLED 1

#define SPEED 9600                      // baudrate               
#define INTERVAL 120                    // 12ms setup in CF
#define T240 250                        // wait for next send after get answer
#define MAX_TRY_COUNT 15                // max count send packet if have problem in sending
#define TIMEOUT 3000                    // timeout for reading 300 ms

#define REEL_ID        0x01       // address for reel driver
#define LAYER_ID       0x02       // address for layer driver
#define INSTRUMENT_ID  0x03       // address for instrument
#define ALL            0x00       // address for all devices

#define MAX_SIZE_QUEUE 64

#define K 0.4                   //multiply component make link with potentiometers 
#define DELTA 99                 //threashold to change value potentiometer,
                                //I have noise on ADC inputs, need use filter for power ADC 

#define OVERLOAD  PD3            // INT0 interrupt connected
#define STOP      PD2                // INT1 interrupt connected

#define RIGHT     PD6               // layer direction right button
#define LEFT      PB1                // layer direction left button

#define UP        PC3                   // reel direction up button
#define LED_UP    PC2               //LED in button

#define DOWN      PC1                 //reel direction down button
#define LED_DOWN  PC0             //LED in button

#define ADC_REEL  21             // potentiometr pin for control speed on reel motor
#define ADC_LAYER 20            // potentiometr pin for control speed on layer motor

#define STOP_ALL              0    
#define START_ALL_CW          1 
#define START_ALL_CCW         2
#define LAYER_START_MAX_CW    3
#define LAYER_START_MAX_CCW   4
#define LAYER_STOP_SET_SPEED  5
#define LAYER_SET_SPEED       6
#define REEL_SET_SPEED        7

#define MAX_REQUESTS 24

#endif