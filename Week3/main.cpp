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

int setSpeed(unsigned int j);

void emergencyButtonInit();

int direction = 0;
uint16_t speed = 0;
uint16_t x = 0;

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
	emergencyButtonInit();

	sei();

	while(1)
	{
		ledIndicatorActive();
		dataint = USART_Receive();
		//USART_Transmit(dataint);
		switch (dataint){
			case 'w':
			x++;
			direction = 0;
			speed = setSpeed(x);
			motorControlLeft(direction, speed);
			motorControlRight(direction, speed);
			//_delay_ms(1000);
			//ledIndicatorIdle();
			//USART_Transmit('f');
			break;
			case 32:
			speed = 0;
			x = 0;
			motorControlRight(0, 0);
			motorControlLeft(0, 0);
			//ledIndicatorActive();
			break;
			case 's':
			//x--;
			if (direction == 0){
				direction = 0;
				speed = speed/2;
				x--;
			}
			if (speed <= 8192){
				motorStop();
				speed = 0;
				x = 0;
			}
			if (speed == 0){
				direction = 1;
				speed = 32767;
			}
			//speed = setSpeed(x);
			motorControlLeft(direction, speed);
			motorControlRight(direction, speed);
			break;
			case 'a':
			//speed = setSpeed(x);
			motorControlRight(direction, speed);
			speed = speed/2;
			motorControlLeft(direction, speed);
			break;
			case 'd':
			//speed = setSpeed(x);
			motorControlLeft(direction, speed);
			speed = speed/2;
			motorControlRight(direction, speed);
			break;
			// case '1':
			// 	x = 32767/2;
			// 	speed = x;
			default:
			break;
		}

	}
	for(;;);
}

void emergencyButtonInit()
{
	PCICR = (1<<PCIE0);
	PCMSK0 = (1<<PCINT0);
	PCIFR = (1<<PCIF0);
	PINB = (0<<PINB0);
}

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

int setSpeed(unsigned int j){
	switch (j){
		case 0:
		return 0;
		break;
		case 1:
		return 65535/4;
		break;
		case 2:
		return 65535/2;
		break;
		case 3:
		return (65535/4)*3;
		break;
		//case 4:
		//	return 65535;
		//	break;
		default:
		x = 0;
		break;
	}
	return 0;
}

ISR(USART1_RX_vect){
	if(UDR1 == '1'){
		speed = 65535;
	}
}

// ISR(PCINT0_vect)
// {
// 	if(PINB | (1<<0)) {
// 		// _delay_ms(500);
// 		emergencyButton();
// 	}
// }
