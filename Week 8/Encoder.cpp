//#include "mrBeen.h"

void initEncoder();
void sendDistance();
void setDistance();
void dist();

static volatile uint16_t count = 0;
static volatile uint16_t distance = 0;

void sendDistance(){			//stuurt distance + " CM" via USART naar laptop
	//if(count % 50 == 0){
	writeInt(distance);
	writeString(" Cm");
	USART_Transmit('\n');
	USART_Transmit('\r');
	//}
}

void setDistance(){			// distance = afstand in cm, en count = tellen uit de encoder. count wordt na x keer tellen omgezet in distance 
	if(count % 50 == 0){
		distance++;
	}
}

void dist()
{
	count += 1;
	setDistance();
}


void initEncoder(){			// initialiseer encoders, zet count op 0
	
	PCICR = (1<<PCIE0);		//pin change interrupt enable
	PCMSK0 = (1<<PCINT4);		//enable pin change interrupt pin 4
	PCMSK0 = (1<<PCINT6);		//enable pin change interrupt pin 6
	EIMSK = (1<<INT6);		//set interrupt mask
	PCIFR = (1<<PCIF0);		//set pin change interrupt flag register
	PORTE = (0<<PORTE6);		//set right encoder
	PINB = (0<<PINB4);		//set left encoder
	EICRB = (0<<ISC60);		//external interrupt control register
	EICRB = (1<<ISC61);			
	count = 0;			//count = 0 bij start-up
}
