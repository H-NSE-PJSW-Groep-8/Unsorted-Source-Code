#define F_CPU 16000000ul

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>

//##############################################################################
// user defines

#define TWI_START_SUCCES	0x08
#define TWI_RESTART_SUCCES	0x10
#define TWI_ADDR_ACK		0x18
#define TWI_ADDR_NACK		0x20
#define TWI_DATA_ACK		0x28

#define SDS1307_R			0xD7 // (kan d4, d5 of d6 zijn)
#define SDS1307_W			0xD6 // (kan d4, d5 of d6 zijn)
#define SDS1307_CTRL_5		0x24
#define SDS1307_GYRO_MULTI	0x68
#define SDS1307_GYRO_X_L	0x28
#define SDS1307_GYRO_X_H	0x29
#define SDS1307_GYRO_Y_L	0x2A
#define SDS1307_GYRO_Y_H	0x2B
#define SDS1307_GYRO_Z_L	0x2C
#define SDS1307_GYRO_Z_H	0x2D

//##############################################################################
// global variables

uint8_t data_Read;
uint8_t address;

//##############################################################################
// prototype functions

// twi functions
void twiInit();
void twiStart();
void twiSendByte(uint8_t byte);
void twiReadByte();
uint8_t twiStatus();
void twiStop();

// usart1 functions
void usart1Init();
void usart1SendChar(char x);
void usart1SendString(const char st[]);
void usart1SendInt(int16_t val);

// gyro functions
void gyroWrite(uint8_t reg_addr, uint8_t data);
void gyroRead(uint8_t addr);

//##############################################################################
// main loop

int main()
{
	twiInit();
	
	usart1Init();
	usart1SendChar('d');
	
	gyroWrite(0x20, 0b1111);
	gyroWrite(SDS1307_CTRL_5, (1<<7));
	while (1)
	{
		
		usart1SendString("Gyro = {\n");
			
		gyroRead(SDS1307_GYRO_X_H);
		usart1SendInt(data_Read);
		usart1SendChar('\t');

		gyroRead(SDS1307_GYRO_Y_H);
		usart1SendInt(data_Read);
		usart1SendChar('\t');

		gyroRead(SDS1307_GYRO_Z_H);
		usart1SendInt(data_Read);
		usart1SendChar('\t');
			
		usart1SendString("}\n\r");
	}
}


//##############################################################################
// twi functions

void twiInit()
{
	PORTD |= (1<<1) | (1<<0);
	TWBR = 8;
	TWCR = (1 << TWEN);
}

void twiStart()
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

void twiSendByte(uint8_t byte)
{
	TWDR = byte;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

void twiReadByte()
{
	
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	
	data_Read = TWDR;
	
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));

}

uint8_t twiStatus()
{
	return TWSR & 0xF8;
}

void twiStop()
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	while(TWCR & (1<<TWSTO));
}

//##############################################################################
// usart1 functions

void usart1Init()
{
	UBRR1H = 0;
	UBRR1L = 103;
	UCSR1B |= (1 << RXEN1) | (1 << TXEN1);
	UCSR1C |= (1 << UCSZ10) | (1 << UCSZ11);
}

void usart1SendChar(char x)
{
	while (!(UCSR1A & (1 << UDRE1)));
	UDR1 = x;
}

void usart1SendString(const char st[])
{
	uint8_t i;
	for (i = 0 ; st[i] != 0 ; i++)
	usart1SendChar(st[i]);
}

void usart1SendInt(int16_t val)
{
	char buffer[8];
	itoa(val, buffer, 10);
	usart1SendString(buffer);
}

//##############################################################################
// writer reader functions

void gyroWrite(uint8_t reg_addr, uint8_t data)
{
	twiStart();
	if(twiStatus() != TWI_START_SUCCES)
	{
		usart1SendString("Failed to init TWI");
		return;
	}
	
	twiSendByte(SDS1307_W);
	if(twiStatus() != TWI_ADDR_ACK)
	{
		usart1SendString("Failed to find device");
		return;
	}

	twiSendByte(reg_addr);
	if(twiStatus() != TWI_DATA_ACK)
	{
		usart1SendString("Failed to write register addr");
		return;
	}
	
	twiSendByte(data);
	if(twiStatus() != TWI_DATA_ACK)
	{
		usart1SendString("Failed to write data");
		return;
	}
	
	twiStop();
}

void gyroRead(uint8_t addr){
	twiInit();
	twiStart();
	//if(twiStatus() != TWI_START_SUCCES)
	//{
		//usart1SendString("Failed to init TWI");
		//return;
	//}
	twiSendByte(SDS1307_W);
	//if(twiStatus() != TWI_DATA_ACK)
	//{
	//usart1SendString("Failed to write data");
	//return;
	//}
	twiSendByte(addr);
	//if(twiStatus() != TWI_DATA_ACK)
	//{
	//usart1SendString("Failed to write data");
	//return;
	//}
	twiStart();
	twiSendByte(SDS1307_R);
	//if(twiStatus() != TWI_DATA_ACK)
	//{
		//usart1SendString("Failed to write data");
		//return;
	//}
	twiReadByte();
	twiStop();
}
