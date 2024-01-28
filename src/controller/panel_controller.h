#pragma once
#ifndef _PANEL_CONTROLLER_H_
#define _PANEL_CONTROLLER_H_

#include <stdint.h>
#include <config/settings.h>
#include <model/queue.h>

extern "C" {
#if DEBUG_ENABLED
    #include <config/debug_cfg.h>
#endif
    #include <config/adc_cfg.h>
}

class PanelController {
private:
    bool winch_running = false;
    bool click = false;
    uint64_t btn_timer = 0;

    volatile uint16_t reel_pot_prev  = 0;
    volatile uint16_t layer_pot_prev = 0;

    void init_port(){
        DDRC &= ~(1 << UP);             // Set UP pin as input
        PORTC |= (1 << UP);             // Enable pull-up resistor for UP pin

        DDRC &= ~(1 << DOWN);           // Set DOWN pin as input
        PORTC |= (1 << DOWN);           // Enable pull-up resistor for DOWN pin

        DDRB &= ~(1 << LEFT);           // Set LEFT pin as input
        PORTB |= (1 << LEFT);           // Enable pull-up resistor for LEFT pin

        DDRD &= ~(1 << RIGHT);          // Set RIGHT pin as input
        PORTD |= (1 << RIGHT);          // Enable pull-up resistor for RIGHT pin

        DDRD &= ~(1 << STOP);           // Set STOP pin as input

        DDRD &= ~(1 << OVERLOAD);       // Set OVERLOAD pin as input
        
        DDRC &= ~(1 << ADC_REEL);       // Set ADC_REEL pin as input
        DDRC &= ~(1 << ADC_LAYER);      // Set ADC_LAYER pin as input   
    }


protected:
    Queue queue;
public:
    inline void init_panel(){
        init_port();
        init_adc();
    }

    inline void read_keypad(){
        //if button UP is pressed
        if( ! (PINC & (1 << UP)) && ! winch_running ){

#if DEBUG_ENABLED 
            println(F("UP pressed"));
#endif
            winch_running = true;
        }
        //if button DOWN is pressed
        if( ! (PINC & (1 << DOWN)) && ! winch_running ){

#if DEBUG_ENABLED
            println(F("DOWN pressed"));
#endif
            winch_running = true;
        }
        //if button STOP is pressed
        if( ! (PIND & (1 << STOP)) ){

#if DEBUG_ENABLED
            println(F("STOP pressed"));
#endif
            winch_running = false;
        }

        if( ! (PINB & (1 << LEFT)) ){

#if DEBUG_ENABLED
            println(F("LEFT pressed"));
#endif

            click = true;

        }
        if( ! (PIND & (1 << RIGHT)) ){

#if DEBUG_ENABLED
            println(F("RIGHT pressed"));
#endif

            click = true;
        }

        //if btn LEFT and RIGHT released
        if( (PINB & (1 << LEFT)) && (PIND & (1 << RIGHT)) && click ){
            if(winch_running){
#if DEBUG_ENABLED
            println(F("LEFT/RIGHT released & winch is run"));
#endif                 
            }else{
#if DEBUG_ENABLED
            println(F("LEFT/RIGHT released"));
#endif      
            }
        }




    }

    inline void read_potentiometer(){
        
    }
};


#endif