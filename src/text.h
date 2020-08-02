#pragma once
#include<iostream>
#include<string.h>
#include"/usr/include/GL/glut.h"

class text
{
public:
	char s[150];
	float _x,_y,_z;
	int _type;
	text(void)
	{}

	text(float x,float y,float z,char *t,int type)
	{
		_x=x;
		_y=y;
		_z=z;
		_type=type;
		strcpy(s,t);
	}

	~text(void)
	{}

	void print(){
		int i,len;
		glRasterPos3f(_x,_y,_z);
		len =  strlen(s);
		//draw 1 item at a time
		glDisable(GL_DEPTH_TEST);
		if(_type==1) for (i = 0; i < len; i++) glutBitmapCharacter(GLUT_BITMAP_8_BY_13, s[i]); 
		if(_type==2) for (i = 0; i < len; i++) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
						    
		glEnable(GL_DEPTH_TEST);
	}
};

