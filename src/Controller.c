//Example ATmega2560 Project
//File: ATmega2560Project.c
//An example file for second year mechatronics project

//include this .c file's header file
#include "Controller.h"

//#include "Labs/Lab1.c"
//#include "Labs/Lab2.c"
//#include "Labs/Lab3.c"

//static function prototypes, functions only called in this file



uint8_t mseconds = 0;
uint8_t seconds = 0;
uint8_t minutes = 0;

int main(void)
{
  //variable declarations
	char lcd_string[33] = {0}; //declare and initialise string for LCD

  cli();

  //Initialising timer registers:
  TCCR1A = 0x00;
  //TCCR1B = 0x0A;
  TCCR1B = (1<<WGM12) | (1<<CS11); // Enable CTC mode and prescaler = 8
  TIMSK1 |= (1<<OCIE1A); // Enables the compare mode on channel A

  OCR1A = 19999; // 0x4E1F in hex
  
  //Button interrupts
  DDRD &= ~(1<<PD0 | 1<<PD1);
  EICRA |= (1<<ISC01 | 1<<ISC11); // On falling edge
  EICRA &= ~(1<<ISC00 | 1<<ISC10); // On falling edge
  EIMSK |= (1<<INT0 | 1<<INT1);
  
  
  sei();

	//initialisation section, runs once
	lcd_init(); //initialise 

	_delay_ms(20);

	//main loop
	while(1)
	{	

    lcd_home();       // same as lcd_goto(0);
		lcd_puts("Timer Value:"); //Print string to LCD first line
		lcd_goto( 0x40 );     //Put cursor to first character on second line
		sprintf( lcd_string , "%2u:%2u:%3u" , minutes, seconds, mseconds * 10); 

    lcd_puts( lcd_string );
	}
	return(1);
}//end main 

ISR(INT0_vect)
{
  //Start/Stop button
  TCCR1B *= (1<<CS11);
}

ISR(INT1_vect)
{
  //Reset button
  mseconds = 0;
  seconds = 0;
  minutes = 0;
}

ISR(TIMER1_COMPA_vect)
{
  mseconds += 1;
  
  if (mseconds > 99)
  {
    mseconds = 0;
    seconds += 1;
  }

  if(seconds > 59)
  {
    mseconds = 0;
    seconds = 0;
    minutes += 1;
  }
}