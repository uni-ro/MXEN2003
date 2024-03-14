#include "Controller.h"

#define INCLUDE_MAIN false


uint8_t Blinking_2(uint8_t* port, uint8_t pin1, uint8_t pin2)
{

  _delay_ms(500);     
  *port |= (1 << pin1);
  _delay_ms(500); 
  *port &= ~(1 << pin1);

  _delay_ms(500);     
  *port |= (1 << pin2);
  _delay_ms(500); 
  *port &= ~(1 << pin2);

  return 0;
}

uint8_t BlinkingAlternatively_8(uint8_t* port, int portAmount)
{
 

  for (int i = 0; i < portAmount-1; i++)
  {
    _delay_ms(500);     
    *port |= (1<< i);
    _delay_ms(500); 
    *port &= ~(1<< i);
    
  }
  for (int i = portAmount; i > 0; i--)
  {
        
    *port |= (1<< i);
    _delay_ms(500); 
    *port &= ~(1<< i);
    _delay_ms(500); 
  }

  return 0;
}

#if INCLUDE_MAIN
int main(void)
{
    DDRC = 0xFF; //put PORTC into output mode
    PORTC = 0; 
    
    while(1)
    {
        BlinkingAlternatively_8(&PORTC, 8);
    }
}
#endif