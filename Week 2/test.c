#define F_CPU 16000000
#include avrio.h
#include avrinterrupt.h
#include utildelay.h

#include XBEE_driver.h
#include mrBeen.h

void emergencyButtonInit();
void emergencyButton();

void ledIndicatorInit();
void ledIndicatorActive();
void ledIndicatorIdle();

void motorPwmInit();
void motorControlRight(uint8_t dir, uint16_t power);
void motorControlLeft(uint8_t dir, uint16_t power);
void motorStop();

void initXbee();

xbee_driver xbdh;
uint8_t running;

int main()
{
	ledIndicatorInit();
	motorPwmInit();
	
	_delay_ms(3000);
	
	ledIndicatorActive();
	
	running = 1;
	while(running)
	{
		motorControlRight(0, 65535);
		motorControlLeft(0, 32767);
		_delay_ms(1000);
		motorControlRight(0, 32767);
		motorControlLeft(0, 65535);
		_delay_ms(2000);
		motorControlRight(0, 65535);
		motorControlLeft(0, 32767);
		_delay_ms(1000);
		motorControlRight(1, 32767);
		motorControlLeft(1, 32767);
		_delay_ms(1000);
	}
	for(;;);
}

void emergencyButtonInit()
{
	PCMSK0 = (1PCINT0);
	PCICR = (1PCIE0);
	sei();
}

void emergencyButton()
{
	motorStop();
	ledIndicatorIdle();
	running = 0;
}

void ledIndicatorInit()
{
	DDRC = (17);
	TC4H = 9778;
	OCR4A = (uint8_t)244;
	TCCR4A = (1COM4A1)  (1PWM4A);
	TCCR4B = (0x0F);
	
	ledIndicatorIdle();
}

void ledIndicatorActive()
{
	TC4H = 9778;
	OCR4A = (uint8_t)977;
}

void ledIndicatorIdle()
{
	TC4H = 2448;
	OCR4A = (uint8_t)244;
}

void motorPwmInit()
{
	 timer init PWM_freq = clk_io  (prescaler  TOP)
	ICR1 = 0xFFFF;
	TCCR1A = (1WGM11);
	TCCR1B = (1WGM13)  (1WGM12)  (1CS10);  244Hz-PWM
	
	 init gpio
	DDRB = (16)  (15)  (12)  (11);
}

void motorControlRight(uint8_t dir, uint16_t power)
{
	if(dir) PORTB = (11);
	else PORTB &= ~(11);
	
	if(power)
	{
		TCCR1A = (1COM1A1);
		OCR1A = power;
	}
	else
	{
		TCCR1A &= ~(1COM1A1);
	}
}

void motorControlLeft(uint8_t dir, uint16_t power)
{
	if(dir) PORTB = (12);
	else PORTB &= ~(12);
	
	if(power)
	{
		TCCR1A = (1COM1B1);
		OCR1B = power;
	}
	else
	{
		TCCR1A &= ~(1COM1B1);
	}
}

void motorStop()
{
	motorControlRight(0, 0);
	motorControlLeft(0, 0);
}

//usart
//packet ATPL 04CR

void initXbee()
{
	xbeeSetDefault(&xbdh);
	xbdh.channel = 0x0F;
	xbdh.device_id = 0xFFFF;
	xbdh.power_level = PL_N10DBM;
	
	
	xbeeInit(&xbdh);
}

ISR(PCINT0_vect)
{
	if(PINB & (10)) emergencyButton();
}
