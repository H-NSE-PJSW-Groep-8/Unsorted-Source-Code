//#include "mrBeen.h"

void writeWind();

int16_t turn = 0;
bool turning = 0;
uint8_t gyro_z;



void writeWind(){								//leest gyro_z en verandert de waarde van turning op basis van die data. Output is writeString(windrichting)
	if((gyro_z > 190) & turning){
		turn++;
	}
	if((gyro_z > 50) & (gyro_z < 70) & turning){
		turn--;
	}
	
	if(turn > 76){
		turn = turn%76;
	}
	if(turn < 0){
		turn = turn + 76;
	}
	
	if ((turn > 66) || (turn <= 9)){					//check waarde van turn(dwz de mate van draaiing ten opzicht van beginstand), en output juiste 'windrichting'
		writeString("noord");
	}
	if ((turn > 9) & (turn <= 28)){
		writeString("oost");
	}
	if ((turn > 28) & (turn <= 47)){
		writeString("zuid");
	}
	if ((turn > 47) & (turn <= 66)){
		writeString("west");
	}
	//writeInt(turn);
	writeString("\n\r");
}
