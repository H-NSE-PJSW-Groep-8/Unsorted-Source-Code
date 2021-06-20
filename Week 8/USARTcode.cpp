#include "mrBeen.h"

void USART_Init();
void USART_Transmit(char data);
uint8_t USART_Receive();

void writeString(char st[]);
void writeInt(int i);


void writeString(char st[]){					//schrijf string naar USART
	for(uint8_t i = 0; st[i] !=0; i++){
		USART_Transmit(st[i]);
	}
}

void writeInt(int i){						//schrijf int naar USART
	char buffer[8];
	itoa(i, buffer, 10);
	writeString(buffer);
}

void USART_Init(){						//initialiseer USART
	/* Set baud rate */
	UBRR1 = 103;
	// UBRR1H = (unsigned char)(baud>>8);
	// UBRR1L = (unsigned char)baud;
	/* Enable receiver and transmitter */
	UCSR1B = (1<<RXEN1) | (1<<TXEN1) | (1<<TXCIE1) | (1<<RXCIE1);
	/* Set frame format: 8data, 1 stop bit */
	UCSR1C = (0<<USBS1) | (1<<UCSZ10) | (1<<UCSZ11) | (0<<UCSZ12);
	
	
}

void USART_Transmit(char data){					//zet data in de UDR1
	/* Wait for empty transmit buffer */
	while (~UCSR1A & (1<<UDRE1));
	/* Put data into buffer, sends the data */
	UDR1 = data;
}


uint8_t USART_Receive(){					//haal data op uit UDR1
	while (~UCSR1A & (1<<RXC1));
	return UDR1;
}
