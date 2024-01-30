#include <form/indicator.h>

static uint32_t error_timer = 0; 
static uint8_t led_on_state = 0;

#define BLINK_PERIOD 10000 // period in ms * 0.1

static void init_led() {
    DDRC |= (1 << LED_UP);          // Set LED_UP   pin as output
    DDRC |= (1 << LED_DOWN);        // Set LED_DOWN pin as output
}


void init_indicator() {
    init_led();
}

void up_btn_led_on(){
    PORTC &= ~_BV(LED_DOWN);
    PORTC |= _BV(LED_UP);
}

void down_btn_led_on(){
    PORTC &= ~_BV(LED_UP);
    PORTC |= _BV(LED_DOWN);
}

void led_off(){
    PORTC &= ~_BV(LED_UP);
    PORTC &= ~_BV(LED_DOWN);
}

void error_view(){
  
    PORTC |= _BV(LED_UP);
    PORTC |= _BV(LED_DOWN);
    
}