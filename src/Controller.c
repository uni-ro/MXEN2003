//Example ATmega2560 Project
//File: ATmega2560Project.c
//An example file for second year mechatronics project

//include this .c file's header file
#include "Controller.h"

//static function prototypes, functions only called in this file

uint8_t Blinking_2(uint8_t port)
{

  _delay_ms(500);     
  port |= (1 << PA3);
  _delay_ms(500); 
  port &= ~(1 << PA3);

  _delay_ms(500);     
  port |= (1 << PA4);
  _delay_ms(500); 
  port &= ~(1 << PA4);

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
        
    PORTC |= (1<< i );
    _delay_ms(500); 
    PORTC &= ~(1<< i );
    _delay_ms(500); 
  }

  return 0;
}

int main(void)
{
  DDRC = 0xFF;//put PORTA into output mode
  PORTC = 0; 
  while(1)//main loop
  {
    BlinkingAlternatively_8(&PORTC, 8);

   
  }
  return(1);
}//end main 





