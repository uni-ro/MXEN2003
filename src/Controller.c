//Example ATmega2560 Project
//File: ATmega2560Project.c
//An example file for second year mechatronics project

//include this .c file's header file
#include "Controller.h"

//#include "Labs/Lab1.c"
//#include "Labs/Lab2.c"
//#include "Labs/Lab3.c"

//static function prototypes, functions only called in this file





bool freezeValue = false;

int main(void)
{
  //variable declarations
	char lcd_string[33] = {0}; //declare and initialise string for LCD

  //Initialising interrupt
  cli();
  DDRD &= ~(1<<PD0);
  EICRA |= (1<<ISC01);
  EICRA &= ~(1<<ISC00);
  EIMSK |= (1<<INT0);
  sei();

	//initialisation section, runs once
	adc_init(); //initialse ADC
	lcd_init(); //initialise 

	_delay_ms(20);

	uint16_t variableToPrint;

  uint16_t sensorValue;

  uint16_t distance;

	//main loop
	while(1)
	{	
		
    sensorValue = adc_read(0);

    sensorValue = 5*((sensorValue * 100) / 1023);

    //distance = 39130/(sensorValue*10)-9;
    //distance = 22000 / (sensorValue*10 - 12);
    distance = 2213*10 / (sensorValue*10 - 18*10);

    lcd_home();       // same as lcd_goto(0);
		lcd_puts("Sensor Value:"); //Print string to LCD first line
		lcd_goto( 0x40 );     //Put cursor to first character on second line
		sprintf( lcd_string , "%3u cm" , distance ); 
    //print to string, %u special character to be replaced by variables in later arguments
    if (!freezeValue) {lcd_puts( lcd_string );} //Print string to LCD second line, same as first line
    //%u for unsigned integers, %i,%d for signed integers
    //%lu for long unsigned ...
	}
	return(1);
}//end main 

ISR(INT0_vect)
{
  if (!freezeValue) {freezeValue = true;}
  else {freezeValue = false;}
}
