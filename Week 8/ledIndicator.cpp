#include "mrBeen.h"

void ledIndicatorInit();
void ledIndicatorActive();
void ledIndicatorIdle();

void ledIndicatorInit()					//initialiseer de led
{
DDRC |= (1<<7);
TC4H = 977>>8;
OCR4A = (uint8_t)244;
TCCR4A |= (1<<COM4A1) | (1<<PWM4A);
TCCR4B |= (0x0F);

ledIndicatorIdle();
}

void ledIndicatorActive()				//zet led aan
{
TC4H = 977>>8;
OCR4A = (uint8_t)977;
}

void ledIndicatorIdle()				//zet led uit
{
TC4H = 244>>8;
OCR4A = (uint8_t)244;
}
