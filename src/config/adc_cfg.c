#include <config/adc_cfg.h>

static uint16_t adc_filter(){

}

void init_adc(){
    ADCSRA |= ( 1 << ADEN );
    ADCSRA |= ( 1 << ADPS2 )|( 1 << ADPS1 )|( 1 << ADPS0 );
    ADMUX  |= ( 0 << REFS1 ) | (1 << REFS0 );
    ADMUX  |= (0 << MUX3 ) | (1 << MUX2 ) | (1 << MUX1 ) | (1 << MUX0);
    ADMUX  &= ~(1 << ADLAR);
}


uint16_t adc_read(uint8_t channel){
    uint32_t mid_ADS = 0;
    if( channel == 0){
        ADMUX &= ~(1 << MUX0 );
    }
    else{
        ADMUX |= (1 << MUX0 );
    }
    for(uint8_t i = 0 ; i < 100; i++){
        ADCSRA |= ( 1 << ADSC );
        while(ADCSRA & (1 << ADSC));
        mid_ADS =  mid_ADS + ADC;
    }
    return (uint16_t)(mid_ADS/100.0);
}