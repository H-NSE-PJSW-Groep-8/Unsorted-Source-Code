#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>


#include "i2c.cpp"
#include "ledIndicator.cpp"
#include "USARTcode.cpp"
#include "Encoder.cpp"
#include "Motor.cpp"
#include "Windrichting.cpp"

void Gyro();
void compass();
void checkTurning();

uint16_t setDuty(int8_t speed);

//uint16_t cycle = 1/16000000;
//uint16_t seconde = 16000000

int8_t speed = 0;
uint16_t encodera = 0;
uint16_t encoderb = 0;
uint8_t dataint = 0;
int8_t direction = 0;
uint8_t compassen = 0;
int gyroen = 0;
uint8_t winden = 0;
uint8_t disten = 0;



int main()
{
	twiInit();					//initialiseer USART, Leds, Motors, I2C, en de Encoder
	
	i2cWrite(0x20, 0b1111, SDS1307_W);
	i2cWrite(SDS1307_CTRL_5, (1<<7),SDS1307_W);
	i2cWrite(0x20, 0b1111, compass_w);
	i2cWrite(compass_ctrl_0, (1<<7), compass_w);
	i2cWrite(compass_ctrl_5, 0b01100100, compass_w);
	i2cWrite(compass_ctrl_6, 0b00100000, compass_w);
	i2cWrite(compass_ctrl_7, 0b00000000, compass_w);
	//char udata;
	USART_Init();
	ledIndicatorInit();
	motorPwmInit();
	initEncoder();
	
	//int8_t speed = 0;
	sei();
	while(1)
	{
    
		//dataint = USART_Receive();
		//USART_Transmit(dataint);
		
		//USART_Transmit(dataint);
		switch (dataint)				//check voor commands vanaf laptop en voer uit
		{
			case 'w':
			speed = (speed > 3) ? 4 : speed + 1;
			dataint = 0;
			//writeInt(speed);
			break;
			case 's':
			speed = (speed < -3) ? -4 : speed - 1;
			dataint = 0;
			// if(speed == 0){
			// 	motorStop();
			// }
			break;
			case 'a':
			direction = -1;
			//direction = (direction < 0) ? -1 : direction - 1;
			dataint = 0;
			break;
			case 'd':
			direction = 1;
			//direction = (direction > 0) ? 1 : direction + 1;
			dataint = 0;
			break;
			case ' ':
			//writeString("\n\r");
			//writeInt(encodera);
			//writeString("\n\r");
			//writeInt(encoderb);
			dataint = 0;
			direction = 0;
			speed = 0;
			break;
			case 'p':
			compassen = 1;
			break;
			case '[':
			compassen = 0;
			break;
			case 'g':
			gyroen = 1;
			break;
			case 'h':
			gyroen = 0;
			break;
			case 'k':
			winden = 1;
			break;
			case 'l':
			winden = 0;
			break;
			case 'x':
			disten = 1;
			break;
			case 'c':
			disten = 0;
			default:
			break;
		}
		//dataint = 0;
		uint16_t duty = setDuty(speed);
		if(!speed)
		{
			switch(direction)
			{
				case 0:
				ledIndicatorIdle();
				motorStop();
				break;
				case 1:
				ledIndicatorActive();
				motorControlLeft(0, 32000);
				motorControlRight(1, 32000);
				break;
				case -1:
				ledIndicatorActive();
				motorControlLeft(1, 32000);
				motorControlRight(0, 32000);
				break;
			}
		}
		else
		{
			ledIndicatorActive();
			uint8_t motor_dir = speed < 0;
			switch(direction)
			{
				case 0:
				motorControlLeft(motor_dir, duty);
				motorControlRight(motor_dir, duty);
				break;
				case 1:
				//direction = 0;
				motorControlLeft(motor_dir, duty);
				motorControlRight(motor_dir, duty / 2);
				break;
				case -1:
				//direction = 0;
				motorControlLeft(motor_dir, duty / 2);
				motorControlRight(motor_dir, duty);
				break;
			}
		}
		if(compassen == 1){
			compass();
		}
		if (disten == 1){
		sendDistance();	//Stuur alle data(afstand en richitng) naar laptop
		}
		if (gyroen == 1){
			Gyro();
		}
		checkTurning();
		if (winden == 1){
			writeWind();
		}
	}
}

void checkTurning(){					//Houd bij of Robot draait adhv de waarde van direction.
	if (direction != 0){
		turning = 1;
	}
	else{
		turning = 0;
	}
}

void Gyro(){						//leest Gyroscoopdata en schrijft naar USART
			writeString("Gyro = { ");
				
				i2cRead(SDS1307_GYRO_X_H, SDS1307_W, SDS1307_R);
				writeInt(data_Read);
				writeString("\t");

				i2cRead(SDS1307_GYRO_Y_H, SDS1307_W, SDS1307_R);
				writeInt(data_Read);
				writeString("\t");

				i2cRead(SDS1307_GYRO_Z_H, SDS1307_W, SDS1307_R);
				writeInt(data_Read);
				gyro_z = data_Read;	//zet gyro_z goed voor uitlezen richting.
				writeString(" ");
				
			writeString("}\n\r");
}

void compass(){
			writeString("compass = { ");
				//i2cRead(0x0F, compass_w, compass_r);
				i2cRead(compass_out_x_H, compass_w, compass_r);
				 writeInt(data_Read);
				 writeString("\t");

				i2cRead(compass_out_y_H, compass_w, compass_r);
				writeInt(data_Read);
				writeString("\t");

				i2cRead(compass_out_z_H, compass_w, compass_r);
				writeInt(data_Read);
				writeString(" ");
				
			writeString("}\n\r");
}

uint16_t setDuty(int8_t speed)
{
	if(!speed) return 0;
	return (65535/4) * abs(speed);
}

//ISR's
ISR(USART1_RX_vect){
	/* Get and return received data from buffer */
	//USART_Transmit(UDR1);
	dataint = UDR1;
	UCSR1A = (0 << RXC1) | (0 << TXC1);
}


ISR(INT6_vect){ //right encoder XOR
	if(PORTE | (1<<6)) {
		//_delay_ms(100);
		dist();
	}
}

ISR(PCINT0_vect){ //left encoder XOR
	if(PINB | (1<<4)) {
		//_delay_ms(100);
		//encodera++;
	}
}
