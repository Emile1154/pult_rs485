#pragma once
#ifndef _PANEL_CONTROLLER_H_
#define _PANEL_CONTROLLER_H_


#include <stdint.h>

#include <config/settings.h>
#include <model/modbus_request.h>


#if DEBUG_ENABLED
    #include <config/debug_cfg.h>
#endif
extern "C" {
    #include <config/ms01.h>
    #include <config/adc_cfg.h>
    #include <model/queue.h>
}

#define STOP_STATE  0
/*REEL STATES*/
#define UP_STATE    1
#define DOWN_STATE  2

/*LAYER STATES*/
#define LEFT_STATE  1
#define RIGHT_STATE 2

class PanelController {

private:
    uint8_t winch_running = STOP_STATE;
    uint8_t layer_dir =     STOP_STATE;
    uint8_t clicked = 0;

    uint64_t timer = 0;

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
    }



protected: 
    request_modbus_pack_t * requests[MAX_REQUESTS];
public:
    inline void init_panel(){
        init_port();
        init_adc();
        init_timer();
#if DEBUG_ENABLED
        run_debug();
        println("start");
#endif
    }

    inline void read_keypad(){
        //if button UP is pressed
        if( ! (PINC & (1 << UP)) &&  winch_running == STOP_STATE ){
            push(START_ALL_CW);
#if DEBUG_ENABLED 
            println("UP pressed");
#endif
            winch_running = UP_STATE;
        }
        //if button DOWN is pressed
        if( ! (PINC & (1 << DOWN)) && winch_running == STOP_STATE ){
            push(START_ALL_CCW);
#if DEBUG_ENABLED
            println("DOWN pressed");
#endif
            winch_running = DOWN_STATE;
        }
        //if button STOP is pressed
        if( (PIND & (1 << STOP)) || ! (PIND & (1 << OVERLOAD))){
            push(STOP_ALL);
#if DEBUG_ENABLED
            println("STOP pressed");
#endif
            winch_running = STOP_STATE;
        }

        if( ! (PINB & (1 << LEFT)) && layer_dir == STOP_STATE ){
            push(LAYER_START_MAX_CCW);
#if DEBUG_ENABLED
            println("LEFT pressed");
#endif

            layer_dir = LEFT_STATE;
            clicked = 1;

        }
        if( ! (PIND & (1 << RIGHT)) && layer_dir == STOP_STATE ){
            push(LAYER_START_MAX_CW);
#if DEBUG_ENABLED
            println("RIGHT pressed");
#endif
            layer_dir = RIGHT_STATE;
            clicked = 1;
        }

        //if btn LEFT and RIGHT released
        if( (PINB & (1 << LEFT)) && (PIND & (1 << RIGHT)) && layer_dir && clicked){
            if(winch_running == UP_STATE){
                if(layer_dir == RIGHT_STATE){
                    //only set speed cmd
                    requests[LAYER_SET_SPEED]->set_value(layer_pot_prev, 0);
                    push(LAYER_SET_SPEED);
                }else{
                    // set speed cmd and reverse cmd
                    requests[LAYER_STOP_SET_SPEED]->set_value(layer_pot_prev, 1);
                    push(LAYER_STOP_SET_SPEED);
                    requests[LAYER_REVERSE]->set_value(1,0);
                    push(LAYER_REVERSE);
                    //layer_dir = RIGHT_STATE;
                }
                
#if DEBUG_ENABLED
                println("LEFT/RIGHT released & winch is run");
#endif                 
            }
            if(winch_running == DOWN_STATE){
                if(layer_dir == LEFT_STATE){
                    // only set speed cmd
                    requests[LAYER_SET_SPEED]->set_value(layer_pot_prev, 0);
                    push(LAYER_SET_SPEED);
                }else{
                    //set speed cmd and reverse cmd
                    requests[LAYER_STOP_SET_SPEED]->set_value(layer_pot_prev, 1);
                    push(LAYER_STOP_SET_SPEED);
                    requests[LAYER_REVERSE]->set_value(2,0);
                    push(LAYER_REVERSE);
                    //layer_dir = LEFT_STATE;
                }
#if DEBUG_ENABLED
                println("LEFT/RIGHT released & winch is run");
#endif       
            }
            if(winch_running == STOP_STATE){
#if DEBUG_ENABLED
                println(("LEFT/RIGHT released"));
#endif      
                
            }
            layer_dir = STOP_STATE;
           clicked = 0;
        }
    }

    inline void read_potentiometer(){
        if( get_ms() - timer >= 10000){
            uint16_t reel_value = adc_read(0);
            uint16_t layer_value = adc_read(1) + K*reel_value;
            if(layer_value >= 5000){
                layer_value = 5000;
            }
            if(abs(reel_value - reel_pot_prev) > DELTA){  // if values from potentiometr reel changed refresh freqency on slave devices
                requests[REEL_SET_SPEED]->set_value( reel_value, 0 );
                push(REEL_SET_SPEED);
                reel_pot_prev = reel_value;
#if DEBUG_ENABLED
            print(("reel " ));
            println((reel_value));
#endif                
            } 
            if(abs(layer_value - layer_pot_prev) > DELTA){  // if values from potentiometr layer changed refresh freqency on slave devices
                requests[LAYER_SET_SPEED]->set_value( layer_value, 0 );
                push(LAYER_SET_SPEED);
                layer_pot_prev = layer_value;
#if DEBUG_ENABLED
            print(("Layer " ));
            println((layer_value));
#endif                                
            }
            timer = get_ms();
        }
    }
};
#endif