#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//#include "XBEE_driver.h"
#include "mrBeen.h"

void emergencyButtonInit();
void emergencyButton();

void ledIndicatorInit();
void ledIndicatorActive();
void ledIndicatorIdle();

void motorPwmInit();
void motorControlRight(uint8_t dir, uint16_t power);
void motorControlLeft(uint8_t dir, uint16_t power);
void motorStop();

//void initXbee();
//
//xbee_driver xbdh;
uint8_t running;

int main()
{
	ledIndicatorInit();
	motorPwmInit();
	emergencyButtonInit();

	_delay_ms(3000);

	
	ledIndicatorActive();
	running = 1;
	sei();
	while(running)
	{
		motorControlRight(0, 65535);
		motorControlLeft(0, 32767);
		_delay_ms(1000);
		motorControlRight(1, 32767);
		motorControlLeft(1, 65535);
		_delay_ms(2000);
		// motorControlRight(0, 65535);
		// motorControlLeft(0, 32767);
		// _delay_ms(1000);
		// motorControlRight(1, 32767);
		// motorControlLeft(1, 32767);
		// _delay_ms(1000);
	}
	for(;;);
}

void emergencyButtonInit()
{
	PCICR = (1<<PCIE0);
	PCMSK0 = (1<<PCINT0);
	PCIFR = (1<<PCIF0);
	PINB = (1<<PINB0);
}

void emergencyButton()
{
	if(running == 1){
	motorStop();
	ledIndicatorIdle();
	running = 0;
	_delay_ms(250);
	}
	else{
		running = 1;
		ledIndicatorActive();
		
	}
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

// usart
// packet: "ATPL 04<CR>"

//void initXbee()
//{
	//xbeeSetDefault(&xbdh);
	//xbdh.channel = 0x0F;
	//xbdh.device_id = 0xFFFF;
	//xbdh.power_level = PL_N10DBM;
	//
	//
	//xbeeInit(&xbdh);
//}

ISR(PCINT0_vect)
{
	if(~PINB | (1<<0)) emergencyButton();
}
