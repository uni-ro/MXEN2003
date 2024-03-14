//Example ATmega2560 Project
//File: ATmega2560Project.c
//An example file for second year mechatronics project

//include this .c file's header file
#include "Controller.h"

//#include "Labs/Lab1.c"
//#include "Labs/Lab2.c"
//#include "Labs/Lab3.c"

//static function prototypes, functions only called in this file







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
    Blinking_1(&PORTA, PA1);
  }
  return(1);
}



