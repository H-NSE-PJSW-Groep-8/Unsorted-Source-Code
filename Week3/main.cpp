#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//#include "XBEE_driver.h"
#include "mrBeen.h"

void ledIndicatorInit();
void ledIndicatorActive();
void ledIndicatorIdle();

void USART_Init( unsigned int baud );
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );


//void initXbee();
//
//xbee_driver xbdh;

int main()
{
	char udata;
	USART_Init(9600);
	ledIndicatorInit();
	_delay_ms(3000);
	ledIndicatorActive();

	while(1)
	{
		udata = USART_Receive();
		USART_Transmit(udata);
		USART_Transmit(udata);
	}
	for(;;);
}

// void USART_Init(){
// UBRRn = 103; //baud rate van 9600 bps
// }

void USART_Init( unsigned int baud ){
	/* Set baud rate */
	UBRR1 = 103;
	// UBRR1H = (unsigned char)(baud>>8);
	// UBRR1L = (unsigned char)baud;
	/* Enable receiver and transmitter */
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);
	/* Set frame format: 8data, 1 stop bit */
	UCSR1C = (0<<USBS1)|(1<<UCSZ10) | (1<<UCSZ11) | (0<<UCSZ12);
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR1A & (1<<UDRE1)) );
	/* Put data into buffer, sends the data */
	UDR1 = data;
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR1A & (1<<RXC1)) )
	;
	/* Get and return received data from buffer */
	return UDR1;
}

void ledIndicatorInit()
{
	DDRC |= (1<<7);
	TC4H = 977>>8;
	OCR4A = (uint8_t)244;
	TCCR4A |= (1<<COM4A1) | (1<<PWM4A);
	TCCR4B |= (0x0F);
	
	ledIndicatorIdle();
}

void ledIndicatorActive()
{
	TC4H = 977>>8;
	OCR4A = (uint8_t)977;
}

void ledIndicatorIdle()
{
	TC4H = 244>>8;
	OCR4A = (uint8_t)244;
}

// ISR(PCINT0_vect)
// {
// 	if(PINB | (1<<0)) {
// 		// _delay_ms(500);
// 		emergencyButton();
// 	}
// }
