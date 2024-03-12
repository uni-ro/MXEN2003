//Example ATmega2560 Project
//File: ATmega2560Project.c
//An example file for second year mechatronics project

//include this .c file's header file
#include "Controller.h"

//static function prototypes, functions only called in this file

uint8_t Blinking_1(uint8_t* port)
{

  _delay_ms(500);     
  *port |= (1 << PA1);
  _delay_ms(500); 
  *port &= ~(1 << PA1);

  return 0;
}

uint8_t Blinking_2(uint8_t* port)
{

  _delay_ms(500);     
  *port |= (1 << PA3);
  _delay_ms(500); 
  *port &= ~(1 << PA3);

  _delay_ms(500);     
  *port |= (1 << PA4);
  _delay_ms(500); 
  *port &= ~(1 << PA4);

  return 0;
}

uint8_t BlinkingAlternatively_8(uint8_t* port, int portAmount)
{
 

  for (int i = 0; i < portAmount-1; i++)
  {
    _delay_ms(500);     
    *port |= (1<< i );
    _delay_ms(500); 
    *port &= ~(1<< i );
    
  }
  for (int i = portAmount; i > 0; i--)
  {
        
    *port |= (1<< i );
    _delay_ms(500); 
    *port &= ~(1<< i );
    _delay_ms(500); 
  }

  return 0;
}

int main(void)
{
  cli();
  DDRA = (1<<PA0);
  PORTA = 0;
  DDRD &= ~(1<<PD0);
  //PORTD |= (1<<PD0);
  EICRA |= (1<<ISC01);
  EICRA &= ~(1<<ISC00);
  EIMSK |= (1<<INT0);
  sei();

  while(1)
  {
    
  }
  return(1);
}//end main 

ISR(INT0_vect)
{
  PORTA ^= (1<<PA0);
}




void Part1()
{
  DDRA = (1<<PA1);
  PORTA = 0;

  while(1)
  {
    Blinking_1(&PORTA);
  }
  return(1);
}



