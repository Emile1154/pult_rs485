#include <form/indicator.h>


static void init_led() {
    DDRC |= (1 << LED_UP);          // Set LED_UP   pin as output
    DDRC |= (1 << LED_DOWN);        // Set LED_DOWN pin as output
}


void init_indicator() {
    init_led();
}