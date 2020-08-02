#pragma once
#include <math.h>

class PosVec{
public:
	//variables
	float x,y; 

	//constructors
	PosVec(void){x=0;y=0;}
	PosVec(double _x, double _y){x=_x;y=_y;}
	~PosVec(void){x=0;y=0;}

	//functions
	float magnitude();
	float angle();

	//operators
	friend PosVec operator+(PosVec v1,PosVec v2){
		PosVec v3;
		v3.x = v1.x + v2.x;
		v3.y = v1.y + v2.y;
		return v3;
    }
	friend PosVec operator-(PosVec v1,PosVec v2){
		PosVec v3;
		v3.x = v1.x - v2.x;
		v3.y = v1.y - v2.y;
		return v3;
    }
	friend PosVec operator*(PosVec v1, float num){
		PosVec v2;
		v2.x = v1.x * num;
		v2.y = v1.y * num;
		return v2;
	}
	friend PosVec operator/(PosVec v1, float num){
		PosVec v2;
		v2.x = floor(v1.x / num);
		v2.y = floor(v1.y / num);
		return v2;
	}
	friend bool operator==(PosVec v1,PosVec v2){
		if(v1.x == v2.x && v1.y == v2.y) return true;
		return false;
	}
	friend bool operator!=(PosVec v1,PosVec v2){
		if(v1.x != v2.x	|| v1.y != v2.y) return true;
		return false;
	}
};

