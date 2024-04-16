//Example ATmega2560 Project
//File: ATmega2560Project.c
//An example file for second year mechatronics project

//include this .c file's header file
#include "Robot.h"

//static function prototypes, functions only called in this file

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