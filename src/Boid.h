#pragma once
#include "Main.h"

class PosVec;

class Boid{
	public:
		//Variables
		PosVec pos;
		PosVec speed;
		int radius;
		bool isPredator;
		float angle;

		//Constructors
		Boid();
		Boid(PosVec _pos, PosVec _speed, int _radius, bool isPredator);
		~Boid(void);

		//Functions
		Boid* move(float sepWeight, float alignWeight, float cohWeight);
		Boid* updateBoidNeighbors(std::vector<Boid*> birds);
		Boid* updateBoidNeighbors(std::vector<Boid*> birds, std::vector<Boid*> predators);
		Boid* updateObstacles(std::vector<Boid*> obst, bool useObstacle);
		void draw();
		void draw_obst();
	
	private:
		PosVec seperation();
		PosVec alignment();
		PosVec cohesion();

		std::vector<Boid*> neighbor;
		std::vector<Boid*> obstacle;
		float setAngle(float ang);
};

