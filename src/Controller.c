//Example ATmega2560 Project
//File: ATmega2560Project.c
//An example file for second year mechatronics project

//include this .c file's header file
#include "Controller.h"

//#include "Labs/Lab1.c"
//#include "Labs/Lab2.c"
//#include "Labs/Lab3.c"

//static function prototypes, functions only called in this file

char serial_string[16] = {0};

int main(void)
{
  serial0_init();
  serial2_init();
  milliseconds_init();

  UCSR2B |= (1 << RXCIE2);

  //variable declarations
	char lcd_string[33] = {0}; //declare and initialise string for LCD

  //Initialising interrupt
  cli();
  DDRD &= ~(1<<PD0);
  sei();

  DDRK &= ~(1<<PK6 || 1<<PK7);
  DDRF &= ~(1<<PF0 || 1<<PF1);

	//initialisation section, runs once
	adc_init(); //initialse ADC
	lcd_init(); //initialise 

	_delay_ms(20);

  uint32_t current_ms, last_send_ms;

	uint16_t variableToPrint;

  uint16_t sensorValue;

  uint16_t distance;

  uint8_t axis = 0;

	//main loop
	while(1)
	{	
		current_ms = milliseconds_now();

    if (current_ms - last_send_ms >= 100)
    {
      last_send_ms = milliseconds_now();

      uint16_t leftStick;
      uint16_t rightStick;

      if (axis == 0) {leftStick = adc_read(PK6); rightStick = adc_read(PF1);}
      else {axis = 0; leftStick = adc_read(PK7); rightStick = adc_read(PF0);}

      serial2_write_byte(0xFF); // Start

      serial2_write_byte(0x00 + axis); // Controller and Axis
      
      //_delay_ms(0.1);
      serial2_write_byte(leftStick >> 8); // Data
      serial2_write_byte(leftStick & 0x00FF);

      serial2_write_byte(0x02 + axis); // Controller and Axis
      
      //_delay_ms(0.1);
      serial2_write_byte(rightStick >> 8); // Data
      serial2_write_byte(rightStick & 0x00FF);

      serial2_write_byte(0xFE); // End

      //serial0_print_string(serial_string);

      //sprintf(serial_string, "Val: %u\n", ((rightStick >> 8) << 8) + (rightStick & 0x00FF));
      //serial0_print_string(serial_string);
    }

    sensorValue = (0);

    sensorValue = 5*((sensorValue * 100) / 1023);

    //distance = 39130/(sensorValue*10)-9;
    //distance = 22000 / (sensorValue*10 - 12);
    distance = 2213*10 / (sensorValue*10 - 18*10);

    lcd_home();       // same as lcd_goto(0);
		lcd_puts("Sensor Value:"); //Print string to LCD first line
		lcd_goto( 0x40 );     //Put cursor to first character on second line
		sprintf( lcd_string , "%3u cm" , distance ); 
    //print to string, %u special character to be replaced by variables in later arguments
    //%u for unsigned integers, %i,%d for signed integers
    //%lu for long unsigned ...
	}
	return(1);
}//end main 

/*
ISR(USART2_RX_vect)
{
  static uint8_t serial_fsm_state = 0;

  //sprintf(serial_string, "Val: %u\n", 1);
  //serial0_print_string(serial_string);
}
*/