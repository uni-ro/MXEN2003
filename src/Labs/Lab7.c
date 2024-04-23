#include "Controller.h"

#define INCLUDE_MAIN false

#if INCLUDE_MAIN
    //Controller -----

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

    // -------



    // Robot ------------

    volatile uint16_t leftHorizontalControllerData = 0, leftVerticalControllerData = 0;

volatile uint16_t rightHorizontalControllerData = 0, rightVerticalControllerData = 0;

volatile bool new_message_received_flag = false;
char * serial_string[16] = {0};
int main(void)
{
  serial0_init();
  serial2_init();
  adc_init();
  sei();

  UCSR2B |= (1 << RXCIE2);

  
  milliseconds_init();


  while(1)//main loop
  {
    uint32_t ms_now = milliseconds_now();
    uint32_t ms_prev_send = 0;


    if(!new_message_received_flag)
    {
      if (ms_now - ms_prev_send >= 100)
      {
        ms_prev_send = ms_now;

        sprintf(serial_string, "lHVal: %u\n", leftHorizontalControllerData);
        serial0_print_string(serial_string);
        new_message_received_flag = false;

        serial2_write_byte(0xFF); // Start

        serial2_write_byte(0x5B);

        serial2_write_byte(0xFE); // End
      }
    }
    
    
  }
  return(1);


  //0x0FFC & 0x00FF;
  //0x0FFC >> 8;


}//end main 

uint8_t firstDataByte = 0;

ISR(USART2_RX_vect)
{
  static uint8_t serial_fsm_state = 0;
  uint8_t serial_data_in = UDR2;

  uint8_t* leftController;
  uint8_t* rightController;

  switch(serial_fsm_state)
  {
    case 0:
      break; // Find start byte and do nothing
    case 1:
      if (serial_data_in == 0x00)
      {
        *leftController = &leftHorizontalControllerData;
      }
      else if (serial_data_in == 0x01)
      {
        *leftController = &leftVerticalControllerData;
      }
      serial_fsm_state++;
      break;
    case 2:
      firstDataByte = serial_data_in << 8;
      serial_fsm_state++;
      break;
    case 3:
      firstDataByte += serial_data_in;
      leftHorizontalControllerData = firstDataByte;
      serial_fsm_state++;
      break;

    case 4:
      if (serial_data_in == 0x02)
      {
        *rightController = &rightHorizontalControllerData;
      }
      else if (serial_data_in == 0x03)
      {
        *rightController = &rightVerticalControllerData;
      }
      serial_fsm_state++;
      break;

    case 5:
      firstDataByte = serial_data_in << 8;
      serial_fsm_state++;
      break;
    case 6:
      firstDataByte += serial_data_in;
      rightController = firstDataByte;
      serial_fsm_state++;
      break;

    case 7:


      if (serial_data_in == 0xFE)
      {
        new_message_received_flag = true;
      }

      serial_fsm_state = 0;
      break;
  }

  if (serial_data_in == 0xFF)
  {
    serial_fsm_state = 1;
  }
  
}

    // -----------------
#endif