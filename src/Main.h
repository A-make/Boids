#pragma once
//import standard C librarys
#include <stdio.h> 
#include <stdlib.h>
#include <stdarg.h>
#include <iostream> 
//#include <windows.h>
#include <fstream>
#include <time.h> 
#include <math.h> 
#include <string.h> 
#include <vector>  
#include "/usr/include/GL/glut.h"

//import project headers
#include "PosVec.h"
#include "Boid.h"
#include "text.h"

#define _USE_MATH_DEFINES
#define GRIDSIZE 600
#define MAPSIZE 20
#define BOIDS_NUMBER 100
#define PRED_NUMBER 1
#define UNIT_SPEED 0.3
#define NEIGHBOR_RADIUS 25
#define PREDATORS_NEIGHBOR_RADIUS 40
#define SEPERATION_RADIUS 8
#define ALIGNMENT_RADIUS 15
#define MAX_ANGLE 10
#define PI 3.14159265

using namespace std;
class PosVec;
class Boid;

enum simulation_state {BIRDS, BIRDS_OBST, BIRDS_PRED, BIRDS_OBST_PRED}; 
