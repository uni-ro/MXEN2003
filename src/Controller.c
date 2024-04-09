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
  serial0_init();

  char * serial_string[16] = {0};

  DDRB |= (1<<PB6);
  DDRE |= (1<<PE4);
  adc_init();

  // Init for TCCR1
  TCCR1A |= (1<<WGM10);
  TCCR1B |= (1<<WGM13);

  TCCR1A |= (1<<COM1B0 | 1<<COM1B1);
  TCCR1B |= (1<<CS11);

  // Init for TCCR3
  TCCR3A |= (1<<WGM30);
  TCCR3B |= (1<<WGM33);

  TCCR3A |= (1<<COM3B0 | 1<<COM3B1);
  TCCR3B |= (1<<CS31);

  //OCR1B = 19080;  // The COMP value for 620us (0 degrees)
  OCR1B = 18030; // The COMP value for 2420us (180 degrees)
  OCR1A = 20000;

  OCR3B = 18030;
  OCR3A = 20000;

	//main loop
	while(1)
	{	
    uint32_t xVal = adc_read(PF0);
    uint32_t yVal = adc_read(PF1);

    xVal = (xVal * 1050 / 1023) + 18030; 
    yVal = (yVal * 1050 / 1023) + 18030; 

    sprintf(serial_string, "xVal: %u\t", xVal);
    serial0_print_string(serial_string);

    sprintf(serial_string, "yVal: %u\n", yVal);
    serial0_print_string(serial_string);

    OCR1B = xVal;
    OCR3B = yVal;
	}
	return(1);
}//end main 