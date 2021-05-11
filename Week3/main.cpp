#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//#include "XBEE_driver.h"
#include "mrBeen.h"

void ledIndicatorInit();
void ledIndicatorActive();
void ledIndicatorIdle();

void motorPwmInit();
void motorControlRight(uint8_t dir, uint16_t power);
void motorControlLeft(uint8_t dir, uint16_t power);
void motorStop();

void USART_Init( unsigned int baud );
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );


//void initXbee();
//
//xbee_driver xbdh;

int main()
{
	//char udata;
	int dataint;
	USART_Init(9600);
	ledIndicatorInit();
	_delay_ms(3000);
	ledIndicatorActive();
	motorPwmInit();

	int direction = 0;
	int speed = 0;

	while(1)
	{
		ledIndicatorActive();
		dataint = USART_Receive();
		USART_Transmit(dataint);
		switch (dataint){
		case 'w':
			direction = 0;
			motorControlLeft(direction, 65535);
			motorControlRight(direction, 65535);
			//_delay_ms(1000);
			ledIndicatorIdle();
			//USART_Transmit('f');
			break;
		case 32:
			motorControlRight(0, 0);
			motorControlLeft(0, 0);
			//ledIndicatorActive();
			break;
		case 's':
			direction = 1;
			motorControlLeft(direction,32767);
			motorControlRight(direction, 32767);
			break;
		case 'a':
			motorControlRight(direction, 32767);
			motorControlLeft(direction, 0);
			break;
		case 'd':
			motorControlLeft(direction, 32767);
			motorControlRight(direction, 0);
			break;
		default:
			break;
		}

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

void motorPwmInit()
{
	// timer init PWM_freq = clk_io / (prescaler * TOP)
	ICR1 = 0xFFFF;
	TCCR1A |= (1<<WGM11);
	TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS10); // 244Hz-PWM
	
	// init gpio
	DDRB |= (1<<6) | (1<<5) | (1<<2) | (1<<1);
}

void motorControlRight(uint8_t dir, uint16_t power)
{
	if(dir) PORTB |= (1<<1);
	else PORTB &= ~(1<<1);
	
	if(power)
	{
		TCCR1A |= (1<<COM1A1);
		OCR1A = power;
	}
	else
	{
		TCCR1A &= ~(1<<COM1A1);
	}
}

void motorControlLeft(uint8_t dir, uint16_t power)
{
	if(dir) PORTB |= (1<<2);
	else PORTB &= ~(1<<2);
	
	if(power)
	{
		TCCR1A |= (1<<COM1B1);
		OCR1B = power;
	}
	else
	{
		TCCR1A &= ~(1<<COM1B1);
	}
}

void motorStop()
{
	motorControlRight(0, 0);
	motorControlLeft(0, 0);
}

// ISR(PCINT0_vect)
// {
// 	if(PINB | (1<<0)) {
// 		// _delay_ms(500);
// 		emergencyButton();
// 	}
// }
