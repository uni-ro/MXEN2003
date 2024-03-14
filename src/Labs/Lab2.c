#include "Controller.h"

#define INCLUDE_MAIN false


#if INCLUDE_MAIN
int main()
{
    DDRA = 0;//put PORTA into output mode
    PORTA = 1; 
    bool counter = 0;

    DDRC = 0xFF;
    PORTC = 0;

    while(1)//main loop
    {
        //BlinkingAlternatively_8(&PORTC, 8);

        bool button_pushed = PINA & (1<<PINA0);

        if (!button_pushed && counter == 0) {counter = 1; _delay_ms(400);}
        else if (!button_pushed && counter == 1) {counter = 0; _delay_ms(400);}

        adc_init();
        _delay_ms(20);

        uint16_t value_x = adc_read(0);
        uint16_t value_y = adc_read(1);
        uint8_t value_therm = adc_read(2);

        uint8_t pin = 0;

        uint16_t value = value_x;

        if (counter == 1) value = value_y;
        else value = value_x;

        if (value < 128){
        pin = PC0;
        } else if (128 <= value && value < 256){
        pin = PC1;
        } else if (256 <= value && value < 384){
        pin = PC2;
        } else if (384 <= value && value < 512){
        pin = PC3;
        } else if (512 <= value && value < 640){
        pin = PC4;
        } else if (640 <= value && value < 768){
        pin = PC5;
        } else if (768 <= value && value < 890){
        pin = PC6;
        } else if (890 <= value && value < 1023){
        pin = PC7;
        }

        /*
        if ((value_x & (1 << PA7)) == (1<<PA7)){
        pin = PC7;
        } else if ((value_x & (1 << PA6)) == (1<<PA6)){
        pin = PC6;
        } else if ((value_x & (1 << PA5)) == (1<<PA5)){
        pin = PC5;
        } else if ((value_x & (1 << PA4)) == (1<<PA4)){
        pin = PC4;
        } else if ((value_x & (1 << PA3)) == (1<<PA3)){
        pin = PC3;
        } else if ((value_x & (1 << PA2)) == (1<<PA2)){
        pin = PC2;
        } else if ((value_x & (1 << PA1)) == (1<<PA1)){
        pin = PC1;
        } else if ((value_x & (1 << PA0)) == (1<<PA0)){
        pin = PC0;
        }*/

        //if (counter == 1) PORTC = value_y;
        //else PORTC = value_x;

        if (counter == 1) PORTC = (1<<pin);
        else PORTC = (1<<pin);
        //PORTC = value_therm;
    }
    
    return 1;
}
#endif