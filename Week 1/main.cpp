#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>

#include "mrBeen.h"

void timer4Init();

int main()
{
	timer4Init();

	while(1)
	{
		// do nothing
	}

}

void timer4Init()
{	
	// Hoe ben ik op 977 gekomen: 16000000 / 16384 = 976.5625 .977 = 0.999 HZ
	DDRC |= (1<<7);							//PortC7 naar output
	TC4H = 977>>8;							// Registers zijn 8-bit groot en de timer is 10 bit, dus TC4H = 0b000000xx
	OCR4C = (uint8_t)977;						// OCR4A = 0b11111111 (Totaal = bijv: 0b11-11111111
	TC4H = 244>>8;							// 25% duty cycle
	OCR4A = (uint8_t)244;						// 25% duty cycle
	TCCR4A |= (1<<COM4A1) | (1<<PWM4A);				// Fast PWM mode cleared on compare match. Set when TCNT4 = 0x000, Puls width modulator A enable
	TCCR4B |= (0x0F);						// Prescaler van 16384
}
