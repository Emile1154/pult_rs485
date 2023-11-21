#include <Panel.h>
#include <millis.h>
uint16_t Panel::layer_value = 0; 
uint16_t Panel::reel_value = 0;

uint32_t timer = 0;
Panel::Panel(
    const CommandInterface * upCommand,
    const CommandInterface * downCommand,
    const CommandInterface * leftCommand,
    const CommandInterface * rightCommand,
    const CommandInterface * stopCommand,
    const CommandInterface * layerStopCommand,
    const CommandInterface * reelSetSpeedCommand,
    const CommandInterface * layerSetSpeedCommand
)
{
    this->upCommand = upCommand;
    this->downCommand = downCommand;
    this->leftCommand = leftCommand;
    this->rightCommand = rightCommand;
    this->stopCommand = stopCommand;
    this->layerStopCommand = layerStopCommand;
    this->reelSetSpeedCommand = reelSetSpeedCommand;
    this->layerSetSpeedCommand = layerSetSpeedCommand;
}
    
Panel::~Panel()
{

}

void Panel::query(){
    if(millis() - timer >= 120){
        const CommandInterface * command = queue.pop();
        if(command != nullptr){
            command->execute();   
            command = nullptr;
        }
        timer = millis();
    }
}

void Panel::initPins(){
    DDRC &= ~(1 << UP);             // Set UP pin as input
    PORTC |= (1 << UP);             // Enable pull-up resistor for UP pin

    DDRC &= ~(1 << DOWN);           // Set DOWN pin as input
    PORTC |= (1 << DOWN);           // Enable pull-up resistor for DOWN pin

    DDRC |= (1 << LED_UP);          // Set LED_UP pin as output

    DDRC |= (1 << LED_DOWN);        // Set LED_DOWN pin as output

    DDRB &= ~(1 << LEFT);           // Set LEFT pin as input
    PORTB |= (1 << LEFT);           // Enable pull-up resistor for LEFT pin

    DDRD &= ~(1 << RIGHT);          // Set RIGHT pin as input
    PORTD |= (1 << RIGHT);          // Enable pull-up resistor for RIGHT pin

    DDRD &= ~(1 << STOP);           // Set STOP pin as input

    DDRD &= ~(1 << OVERLOAD);       // Set OVERLOAD pin as input
    
    DDRC &= ~(1 << ADC_REEL);       // Set ADC_REEL pin as input
    DDRC &= ~(1 << ADC_LAYER);      // Set ADC_LAYER pin as input
    
}

void Panel::initADC(){
    ADMUX |= (1 << REFS0);  //ref voltage setup
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //f = 125 kHz
    ADCSRA |= (1 << ADEN); //enable adc
}

uint16_t Panel::readADC(uint8_t _pin){
    ADMUX = (ADMUX & 0xF0) | (_pin & 0x0F); //select pin
    ADCSRA |= (1 << ADSC);                  //start convert
    while (ADCSRA & (1 << ADSC));           //wait end convertion
    return ADC;
}

void Panel::checkKeyboard(){
    static bool changed;
    static bool click;
    static uint32_t btnTimer;
    static uint32_t btnTimer2;
    static bool _click;
    if(!(PINC & (1 << UP)) && !changed){
        queue.push(upCommand);
        changed = true;            // forbidding flag
        PORTC |= (1 << LED_UP);
    }
    else if(!(PINC & (1 << DOWN)) && !changed){
        queue.push(downCommand);
        changed = true;             // forbidding flag
        PORTC |= (1 << LED_DOWN);
    }
    else if(!(PINB & (1 << LEFT)) && !click && millis() - btnTimer > 70){
        queue.push(leftCommand);
        btnTimer = millis();
        click = true;
    } 
    else if(!(PIND & (1 << RIGHT)) && !click && millis() - btnTimer > 70){
        queue.push(rightCommand); 
        btnTimer = millis();
        click = true;
    }
    else if(click && (PIND & (1 << RIGHT)) && (PINB & (1 << LEFT)) && millis() - btnTimer > 70){ 
        queue.push(layerStopCommand);
        btnTimer = millis();
        click = false;
    }
    else if( (!(PIND & (1 << STOP)) && !_click && millis() - btnTimer2 > 100) || !(PIND & (1 << OVERLOAD)) ){
        queue.push(stopCommand);
        PORTC &= ~(1 << LED_UP);
        PORTC &= ~(1 << LED_DOWN);
        btnTimer2 = millis();
        changed = false;
        click = true;
    }
    else if( (PIND & (1 << STOP)) && _click && millis() - btnTimer2 > 100 ){
        btnTimer2 = millis();
        click = false;
    }
}

uint16_t Panel::filter(uint8_t _pin, uint16_t component){
  int sum = 0;
  for(uint8_t i = 0; i < 16; i++){
    sum += readADC(_pin) + component;
  }
  return sum >> 4;
}

void Panel::checkPots(){
    static volatile uint16_t old_reel_value;
    static volatile uint16_t old_layer_value;
    static uint32_t wait;
    uint16_t reel_pot = filter(ADC_REEL, 0);
    reel_value = map(reel_pot,140, 1023, 0, 5000);
    if(reel_value > 5000){reel_value = 5000;}
    if(reel_value <= 0) {reel_value = 0;}  
    
    uint16_t layer_pot = filter(ADC_LAYER, K*reel_pot);
    layer_value = map(layer_pot, 60, 1023, 0, 5000); 
    if(layer_value > 5000){ layer_value = 5000;}
    if(layer_value <= 0) { layer_value = 0;}
    if(millis() - wait >= 40){
        if(abs(reel_value - old_reel_value) > DELTA){  // if values from potentiometr reel changed refresh freqency on slave devices
            queue.push(reelSetSpeedCommand);
            old_reel_value = Panel::reel_value;
        } 
        if(abs(layer_value - old_layer_value) > DELTA){  // if values from potentiometr layer changed refresh freqency on slave devices
            queue.push(layerSetSpeedCommand);
            old_layer_value = Panel::layer_value;
        }
        wait = millis();
    }
}
