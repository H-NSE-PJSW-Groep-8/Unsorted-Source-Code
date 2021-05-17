#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//#include "XBEE_driver.h"
#include "mrBeen.h"

//void emergencyButtonInit();
//void emergencyButton();

void initEncoder();
void dist();

void ledIndicatorInit();
void ledIndicatorActive();
void ledIndicatorIdle();

void motorPwmInit();
void motorControlRight(uint8_t dir, uint16_t power);
void motorControlLeft(uint8_t dir, uint16_t power);
void motorStop();

uint8_t running = 1;
static volatile uint16_t count;

int main()
{
	ledIndicatorInit();
	motorPwmInit();
	//emergencyButtonInit();
	
	initEncoder();
	sei();
	
	_delay_ms(3000);
	
	ledIndicatorActive();

	
	while(running)
	{
		motorControlRight(0, 32767);
		motorControlLeft(0, 32767);
		
		//if(count > 200){
		//motorStop();
		//_delay_ms(6000);
		//count = 0;
		//}
	}
	for(;;);
}

void initEncoder(){
	PCICR = (1<<PCIE0);
	PCMSK0 = (1<<PCINT4);
	PCIFR = (1<<PCIF0);
	
	PORTB = (1<<PORTB4);
	count = 0;
}

void dist()
{
	count += 1;

}



//void emergencyButtonInit()
//{
	//PCICR = (1<<PCIE0);
	//PCMSK0 = (1<<PCINT0);
	//PCIFR = (1<<PCIF0);
	//PINB = (0<<PINB0);
//}
//
//void emergencyButton()
//{
	//if(running == 1){
		//motorStop();
		//ledIndicatorIdle();
		//running = 0;
		//_delay_ms(250);
	//}
	//else{
		//running = 1;
		//ledIndicatorActive();
//
	//}
//}

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

ISR(PCINT0_vect) //left encoder XOR
{
	if(PINB | (1<<4)) {
		_delay_ms(50);
		dist();
	}
}

//ISR(PCINT0_vect)
//{
	//if(PINB | (1<<0)) {
		////_delay_ms(500);
		//emergencyButton();
	//}
//}
