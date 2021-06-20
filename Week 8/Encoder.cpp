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
