#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

void ledIndicatorInit();
void ledIndicatorActive();
void ledIndicatorIdle();

void motorPwmInit();
void motorControlRight(uint8_t dir, uint16_t power);
void motorControlLeft(uint8_t dir, uint16_t power);
void motorStop();
void initEncoder();
void sendDistance();
uint16_t setDuty(int8_t speed);

void USART_Init();
void USART_Transmit(char data);
uint8_t USART_Receive();
void setDistance();

void writeString(char st[]);
void writeInt(int i);
void dist();

int8_t speed;
static volatile uint16_t count;
static volatile uint16_t distance;
uint16_t encodera = 0;
uint16_t encoderb = 0;

int main()
{
	//char udata;
	USART_Init();
	ledIndicatorInit();
	motorPwmInit();
	initEncoder();
	
	speed = 0;

	sei();

	while(1)
	{
		int8_t direction = 0;
		uint8_t dataint = USART_Receive();
		//USART_Transmit(dataint);
		switch (dataint)
		{
		case 'w':
			speed = (speed > 3) ? 4 : speed + 1;
			break;
		case 's':
			speed = (speed < -3) ? -4 : speed - 1;
			// if(speed == 0){
			// 	motorStop();
			// }
			break;
		case 'a':
			direction = -1;
			break;
		case 'd':
			direction = 1;
			break;
		case ' ':
			writeString("\n\r");
			writeInt(encodera);
			writeString("\n\r");
			writeInt(encoderb);
			speed = 0;
			break;
		default:
			break;
		}
		
		uint16_t duty = setDuty(speed);
		if(!speed)
		{
			switch(direction)
			{
			case 0:
				ledIndicatorIdle();
				motorStop();
				break;
			case 1:
				ledIndicatorActive();
				motorControlLeft(0, 32768);
				motorControlRight(1, 32768);
				break;
			case -1:
				ledIndicatorActive();
				motorControlLeft(1, 16768);
				motorControlRight(0, 16768);
				break;
			}
		}
		else
		{
			ledIndicatorActive();
			uint8_t motor_dir = speed < 0;
			switch(direction)
			{
			case 0:
				motorControlLeft(motor_dir, duty);
				motorControlRight(motor_dir, duty);
				break;
			case 1:
				motorControlLeft(motor_dir, duty);
				motorControlRight(motor_dir, duty / 2);
				break;
			case -1:
				motorControlLeft(motor_dir, duty / 2);
				motorControlRight(motor_dir, duty);
				break;
			}
		}
	}
}

void writeString(char st[]){
	for(uint8_t i = 0; st[i] !=0; i++){
		USART_Transmit(st[i]);
	}
}

void writeInt(int i){
	char buffer[8];
	itoa(i, buffer, 10);
	writeString(buffer);
}

void dist()
{
	count += 1;
	setDistance();
	sendDistance();
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

void initEncoder(){
	
	PCICR = (1<<PCIE0);
	PCMSK0 = (1<<PCINT4);
	PCMSK0 = (1<<PCINT6);
	EIMSK = (1<<INT6);
	PCIFR = (1<<PCIF0);
	PORTE = (0<<PORTE6);
	PINB = (0<<PINB4);
	EICRB = (0<<ISC60);
	EICRB = (1<<ISC61);
	count = 0;
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

void sendDistance(){
	if(count % 100 == 0){
		writeInt(distance);
		writeString(" Cm");
		USART_Transmit('\n');
		USART_Transmit('\r');
	}
}

void setDistance(){
	if(count % 100 == 0){
		distance++;
	}
}

void USART_Init(){
	/* Set baud rate */
	UBRR1 = 103;
	// UBRR1H = (unsigned char)(baud>>8);
	// UBRR1L = (unsigned char)baud;
	/* Enable receiver and transmitter */
	UCSR1B = (1<<RXEN1) | (1<<TXEN1);
	/* Set frame format: 8data, 1 stop bit */
	UCSR1C = (0<<USBS1) | (1<<UCSZ10) | (1<<UCSZ11) | (0<<UCSZ12);
}

void USART_Transmit(char data){
	/* Wait for empty transmit buffer */
	while (~UCSR1A & (1<<UDRE1));
	/* Put data into buffer, sends the data */
	UDR1 = data;
}

uint8_t USART_Receive(){
	/* Wait for data to be received */
	while(~UCSR1A & (1<<RXC1));
	/* Get and return received data from buffer */
	return UDR1;
}

uint16_t setDuty(int8_t speed)
{
	if(!speed) return 0;
	return (65535/4) * abs(speed);
}

ISR(INT6_vect){ //right encoder XOR
	if(PORTE | (1<<6)) {
		//_delay_ms(100);
		dist();
	}
}

ISR(PCINT0_vect){ //left encoder XOR
	if(PINB | (1<<4)) {
		//_delay_ms(100);
		//encodera++;
	}
}