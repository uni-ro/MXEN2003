#include "Controller.h"

#define INCLUDE_MAIN false


uint8_t Blinking_1(uint8_t* port, uint8_t pin)
{

  _delay_ms(500);     
  *port |= (1 << pin);
  _delay_ms(500); 
  *port &= ~(1 << pin);

  return 0;
}

#if INCLUDE_MAIN
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
}

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
    Blinking_1(&PORTA, PA1);
  }
  return(1);
}
#endif