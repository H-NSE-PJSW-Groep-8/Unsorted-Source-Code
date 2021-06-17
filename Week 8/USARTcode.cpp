#include "mrBeen.h"

void USART_Init();
void USART_Transmit(char data);
uint8_t USART_Receive();

void writeString(char st[]);
void writeInt(int i);


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

void USART_Init(){
	/* Set baud rate */
	UBRR1 = 103;
	// UBRR1H = (unsigned char)(baud>>8);
	// UBRR1L = (unsigned char)baud;
	/* Enable receiver and transmitter */
	UCSR1B = (1<<RXEN1) | (1<<TXEN1) | (1<<TXCIE1) | (1<<RXCIE1);
	/* Set frame format: 8data, 1 stop bit */
	UCSR1C = (0<<USBS1) | (1<<UCSZ10) | (1<<UCSZ11) | (0<<UCSZ12);
	
	
}

void USART_Transmit(char data){
	/* Wait for empty transmit buffer */
	while (~UCSR1A & (1<<UDRE1));
	/* Put data into buffer, sends the data */
	UDR1 = data;
}

//void USART_Flush(void){
//unsigned char dummy;
//while ( UCSR1A & (1<<RXC1)) dummy = UDR1;
//}
//
//uint8_t USART_Receive(){
///* Wait for data to be received */
//uint16_t timecount = 0;
//while(~UCSR1A & (1<<RXC1)){
///* Get and return received data from buffer */
//timecount += 1;
////writeInt(timecount);
////writeString("\n");
//if (timecount == 65534){
//USART_Flush();
//break;
//}
//}
//return UDR1;
//}


uint8_t USART_Receive(){
	while (~UCSR1A & (1<<RXC1));
	return UDR1;
}