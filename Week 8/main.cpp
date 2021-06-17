#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>


#include "Gyroscoop.cpp"
#include "ledIndicator.cpp"
#include "USARTcode.cpp"
#include "Encoder.cpp"
#include "Motor.cpp"

void Gyro();

uint16_t setDuty(int8_t speed);

//uint16_t cycle = 1/16000000;
//uint16_t seconde = 16000000

int8_t speed = 0;
uint16_t encodera = 0;
uint16_t encoderb = 0;
uint8_t dataint = 0;
int8_t direction = 0;

int8_t rightturn = 0;
int8_t leftturn = 0;
int8_t windrichting = 0;

void writeWind(int8_t i){
	if (i == 0){
		writeString("noord");
	}
	if (i == 1){
		writeString("oost");
	}
	if (i == 2){
		writeString("zuid");
	}
	if (i == 3){
		writeString("west");
	}
}

void wind(){
		if (rightturn > 35){
			windrichting++;
			rightturn = 0;
		}
		if (leftturn > 35){
			windrichting--;
			leftturn = 0;
		}
		writeWind(windrichting%4);
}

int main()
{
	twiInit();
	
	gyroWrite(0x20, 0b1111);
	gyroWrite(SDS1307_CTRL_5, (1<<7));
	
	//char udata;
	USART_Init();
	ledIndicatorInit();
	motorPwmInit();
	initEncoder();
	
	//int8_t speed = 0;
	sei();
	while(1)
	{
		sendDistance();
		Gyro();
		wind();

		//dataint = USART_Receive();
		//USART_Transmit(dataint);
		
		//USART_Transmit(dataint);
		switch (dataint)
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
	}
}

void Gyro(){
			writeString("Gyro = { ");
				
				gyroRead(SDS1307_GYRO_X_H);
				writeInt(data_Read);
				writeString("\t");

				gyroRead(SDS1307_GYRO_Y_H);
				writeInt(data_Read);
				writeString("\t");

				gyroRead(SDS1307_GYRO_Z_H);
				writeInt(data_Read);
				if(data_Read > 100){
					rightturn++;
				}
				if(data_Read > 20 & data_Read < 30){
					leftturn++;
				}
				writeString(" ");
				
			writeString("}\n\r");
}

ISR(USART1_RX_vect){
	/* Get and return received data from buffer */
	//USART_Transmit(UDR1);
	dataint = UDR1;
	UCSR1A = (0 << RXC1) | (0 << TXC1);
}

uint16_t setDuty(int8_t speed)
{
	if(!speed) return 0;
	return (65535/4) * abs(speed);
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
