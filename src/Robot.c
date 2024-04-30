//Example ATmega2560 Project
//File: ATmega2560Project.c
//An example file for second year mechatronics project

//include this .c file's header file
#include "Robot.h"

//static function prototypes, functions only called in this file

static char serial_string[200] = {0};
volatile uint8_t dataByte1=0, dataByte2=0, dataByte3=0, dataByte4=0, dataByte5=0, dataByte6=0;		// data bytes received
volatile bool new_message_received_flag=false;


int main(void)
{
  
	DDRA = 1;
	DDRB |= (1 << PB6);

	// Init for TCCR1
	TCCR1A |= (1<<WGM10); // Set to mode 9
	TCCR1B |= (1<<WGM13);

	TCCR1A |= (1<<COM1B0 | 1<<COM1B1); // Set to clear on down and set on up
	TCCR1B |= (1<<CS10);

	OCR1B = 10000; // The COMP value for 50% speed
	OCR1A = 20000; // The TOP value for 400Hz

	// Init for TCCR3
	TCCR3A |= (1<<WGM30); // Set to mode 9
	TCCR3B |= (1<<WGM33);

	TCCR3A |= (1<<COM3B0 | 1<<COM3B1); // Set to clear on down and set on up
	TCCR3B |= (1<<CS30);

	OCR3B = 10000; // The COMP value for 50% speed
	OCR3A = 20000; // The TOP value for 400Hz

	// initialisation
	serial0_init(); 	// terminal communication with PC
	serial2_init();		// microcontroller communication to/from another Arduino
	adc_init();			// Initialise ADC
	// or loopback communication to same Arduino
	
	UCSR2B |= (1 << RXCIE2); // Enable the USART Receive Complete interrupt (USART_RXC)
	
	milliseconds_init();
	sei();

	uint16_t batteryVoltage;

	while(1)//main loop
	{

		batteryVoltage = adc_read(0);

		if (batteryVoltage <= 717) // If battery voltage is below 7V (Input Voltage = 3.5V), turn LED On
		{
			PORTA |= (1 << PA4);
		}
		else
		{
			PORTA &= ~(1 << PA4); // Else turn LED Off
		}

		//if a new byte has been received
		if(new_message_received_flag) 
		{

			uint32_t leftStickH = (((uint16_t) dataByte1) << 8) + ((uint16_t) dataByte2);
			uint32_t rightStick = (((uint16_t) dataByte3) << 8) + ((uint16_t) dataByte4);
			uint32_t leftStickV = (((uint16_t) dataByte5) << 8) + ((uint16_t) dataByte6);

					// now that a full message has been received, we can process the whole message
					// the code in this section will implement the result of your message
					//sprintf(serial_string, "received: 1:%4d, 2:%4d , 3:%4d , 4:%4d \n Total: L: %4u\t R: %4u\n", dataByte1, dataByte2, dataByte3, dataByte4, newByte, newByte2);
					//serial0_print_string(serial_string);  // print the received bytes to the USB serial to make sure the right messages are received

			OCR1B = rightStick * 20000 / 1021;
			OCR3B = rightStick * 20000 / 1021;

			sprintf(serial_string, "received: Left Stick Horizontal: %4lu, Left Stick Vertical: %4lu, Right Stick: %lu\n", leftStickH, leftStickV, rightStick);
			serial0_print_string(serial_string);

					new_message_received_flag=false;	// set the flag back to false
			
			if(leftStickV >= 600)//move forward
			{
				PORTA |= ((1<< PA0) | (1<<PA3));
				PORTA &= ~((1<< PA1) | (1<<PA2));
			}
			else if(leftStickV <= 400) //move back
			{
				PORTA |= ((1<< PA1) | (1<<PA2));
				PORTA &= ~((1<< PA0) | (1<<PA3));
			}
			else if(leftStickH >= 600) // move right
			{
				PORTA |= ((1<< PA0) | (1<<PA2));
				PORTA &= ~((1<< PA1) | (1<<PA3));
			}
			else if(leftStickH <= 400)//move left
			{
				PORTA |= ((1<< PA1) | (1<<PA3));
				PORTA &= ~((1<< PA0) | (1<<PA2));
			}
			else 
			{
				PORTA &= ~((1<< PA1) | (1<<PA3));
				PORTA &= ~((1<< PA0) | (1<<PA2));
			}
		}
	}
	return(1);
}//end main 

ISR(USART2_RX_vect)  // ISR executed whenever a new byte is available in the serial buffer
{
	static uint8_t recvByte1=0, recvByte2=0, recvByte3=0, recvByte4=0, recvByte5=0, recvByte6=0;		// data bytes received
	static uint8_t serial_fsm_state=0;									// used in the serial receive ISR
	uint8_t	serial_byte_in = UDR2; //move serial byte into variable
	
	switch(serial_fsm_state) //switch by the current state
	{
		case 0:
		//do nothing, if check after switch case will find start byte and set serial_fsm_state to 1
		break;
		case 1: //waiting for first parameter
		recvByte1 = serial_byte_in;
		serial_fsm_state++;
		break;
		case 2: //waiting for second parameter
		recvByte2 = serial_byte_in;
		serial_fsm_state++;
		break;
		case 3: //waiting for second parameter
		recvByte3 = serial_byte_in;
		serial_fsm_state++;
		break;
		case 4: //waiting for second parameter
		recvByte4 = serial_byte_in;
		serial_fsm_state++;
		break;
    case 5:
    recvByte5 = serial_byte_in;
    serial_fsm_state++;
    break;
    case 6:
    recvByte6 = serial_byte_in;
    serial_fsm_state++;
    break;
		case 7: //waiting for stop byte
		if(serial_byte_in == 0xFE) //stop byte
		{
			// now that the stop byte has been received, set a flag so that the
			// main loop can execute the results of the message
			dataByte1 = recvByte1;
			dataByte2 = recvByte2;
			dataByte3 = recvByte3;
			dataByte4 = recvByte4;
      dataByte5 = recvByte5;
      dataByte6 = recvByte6;
			
			new_message_received_flag=true;
		}
		// if the stop byte is not received, there is an error, so no commands are implemented
		serial_fsm_state = 0; //do nothing next time except check for start byte (below)
		break;
	}
	if(serial_byte_in == 0xFF) //if start byte is received, we go back to expecting the first data byte
	{
		serial_fsm_state=1;
	}
}