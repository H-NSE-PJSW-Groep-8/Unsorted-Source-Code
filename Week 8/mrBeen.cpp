#include "mrBeen.h"

void initialiseerGeleLed() {

   DDRC |= geleLED;  
}
void zetGeleLedAan() {

      PORTC |= geleLED;    //zet led aan
      
}
void zetGeleLedUit() {

       PORTC &= ~geleLED; //zet led uit
}

void zetSnelheidMotorLinks(uint16_t) {

  
}
