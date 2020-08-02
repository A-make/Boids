#include "Main.h"

float PosVec::magnitude(){
	return sqrt(pow(x,2)+pow(y,2));
}

float PosVec::angle(){
	int ang = 0;
	if(x == 0){
		if(y > 0){
			ang = 90;
		} else if(y < 0){
			ang = 270;
		}
	}
	else if(x > 0){
		ang = (atan(y/x) * 180 / PI);
		if(y < 0){
			ang = 360 + ang;
		}
	} 
	else if(x < 0){
		ang = 180 + (atan(y/x) * 180 / PI);
	}
	
	return ang;		
}