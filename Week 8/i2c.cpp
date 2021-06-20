//#define F_CPU 16000000ul

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


#define compass_w			0x3A
#define compass_r			0x3B
#define compass_ctrl_0		0x1F
#define compass_out_x_H		0x09
#define compass_out_y_H		0x0B
#define compass_out_z_H		0x0D
#define compass_ctrl_M		0x12
#define compass_ctrl_3		0x22
#define compass_ctrl_4		0x23
#define compass_ctrl_5		0x24
#define compass_ctrl_6		0x25
#define compass_ctrl_7		0x26
#define compass_fifo_ctrl	0x2E
#define compass_whoami		0x0F

//##############################################################################
// global variables

uint16_t data_Read;
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
void i2cWrite(uint8_t reg_addr, uint8_t data);
void i2cRead(uint8_t addr);


//##############################################################################
// twi functions

void twiInit()										//initialiseer twi
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

void twiSendByte(uint8_t byte)						//zet data(byte) in TWDR om verstuurd te worden
{
	TWDR = byte;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

void twiReadByte()							//haalt waarde van TWDR op en zet in data_Read
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
// writer reader functions

void i2cWrite(uint8_t reg_addr, uint8_t data, uint8_t dev_addr)	//zet data voor de juiste slave adress naar de I2C via twiSendByte()
{
	twiStart();
	//if(twiStatus() != TWI_START_SUCCES)
	//{
		//usart1SendString("Failed to init TWI");
		//return;
	//}
	
	twiSendByte(dev_addr);

	//if(twiStatus() != TWI_ADDR_ACK)
	//{
		//usart1SendString("Failed to find device");
		//return;
	//}

	twiSendByte(reg_addr);
	//if(twiStatus() != TWI_DATA_ACK)
	//{
		//usart1SendString("Failed to write register addr");
		//return;
	//}
	
	twiSendByte(data);
	//if(twiStatus() != TWI_DATA_ACK)
	//{
		//usart1SendString("Failed to write data");
		//return;
	//}
	
	twiStop();
}


void i2cRead(uint8_t addr, uint8_t dev_addr, uint8_t dev_addr_r){		//haal gyroscoopdata van de juiste slave adress uit de I2C via twiReadByte()
	twiInit();
	twiStart();
	//if(twiStatus() != TWI_START_SUCCES)
	//{
		//usart1SendString("Failed to init TWI");
		//return;
	//}
  
	twiSendByte(dev_addr);
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
	twiSendByte(dev_addr_r);

	//if(twiStatus() != TWI_DATA_ACK)
	//{
		//usart1SendString("Failed to write data");
		//return;
	//}
	twiReadByte();
	twiStop();
}
