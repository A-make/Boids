#include "Boid.h"


//----------------------------------------
//Constructors
//----------------------------------------
Boid::Boid(void){
	pos = PosVec(0,0);
	speed = PosVec(1,0);
	radius = 0;
	isPredator = false;
}


Boid::Boid(PosVec _pos, PosVec _speed, int _radius, bool _isPredator){
	pos =_pos;
	angle = _speed.angle();
	speed.x = (UNIT_SPEED * cos(angle*PI/180));
	speed.y = (UNIT_SPEED * sin(angle*PI/180));
	radius = _radius;
	isPredator = _isPredator;
}


Boid::~Boid(void){
	delete this;
}


//----------------------------------------
//Updating functions
//----------------------------------------
Boid* Boid::move(float sepWeight, float alignWeight, float cohWeight){
	speed = speed + (seperation() * sepWeight) + (alignment() * alignWeight) + (cohesion() * cohWeight); 
	angle = setAngle(speed.angle());

	//Unit speed
	speed.x = (UNIT_SPEED * cos(angle*PI/180));
	speed.y = (UNIT_SPEED * sin(angle*PI/180));

	pos = pos + speed;

	//Border wrapping
	if(pos.x > MAPSIZE+2){
		pos.x = -MAPSIZE;
	}
	else if(pos.x < -MAPSIZE-2){
		pos.x = MAPSIZE;
	}
	if(pos.y < -MAPSIZE-2){
		pos.y = MAPSIZE;
	}
	else if (pos.y > MAPSIZE+2){
		pos.y = -MAPSIZE;
	}
	return this;
}


Boid* Boid::updateBoidNeighbors(vector<Boid*> birds){
	float rad;

	while(neighbor.size()>0){ //delete previous neighbor
		neighbor.erase(neighbor.begin());
	}
	for(int i=0;i<birds.size();++i){ //update new neighbor birds
		if (this != birds[i]){
			if((pos - birds[i]->pos).magnitude() < NEIGHBOR_RADIUS){
				neighbor.push_back(birds[i]);
			}
		}
	}
	return this;
}


Boid* Boid::updateBoidNeighbors(vector<Boid*> birds, vector<Boid*> predators){
	float rad;

	if(isPredator) rad = PREDATORS_NEIGHBOR_RADIUS;
	else rad = NEIGHBOR_RADIUS;

	while(neighbor.size()>0){ //delete previous neighbor
		neighbor.erase(neighbor.begin());
	}
	for(int i=0;i<birds.size();++i){ //update new neighbor birds
		if (this != birds[i]){
			if((pos - birds[i]->pos).magnitude() < rad){
				neighbor.push_back(birds[i]);
			}
		}
	}
	for(int i=0;i<predators.size();++i){ //update new neighbor predators
		if ((isPredator && (this != predators[i])) || !isPredator){
			if((pos - predators[i]->pos).magnitude() < (predators[i]->radius + 7)){
				neighbor.push_back(predators[i]);
			}
		}
	}
	return this;
}


Boid* Boid::updateObstacles(vector<Boid*> obst, bool useObstacle){
	while(obstacle.size()>0){ //delete previous obstacles
		//delete obstacle[0];       
		obstacle.erase(obstacle.begin());
	}
	if(useObstacle){
		for(int i=0;i<obst.size();i++){ 
			if((pos - obst[i]->pos).magnitude() < (obst[i]->radius + 7)){
				obstacle.push_back(obst[i]);
			}
		}
	}
	return this;
} 


//----------------------------------------
//Private calculating functions
//-----------------------------------------
PosVec Boid::seperation(){
	PosVec vec(0,0);
	if(!isPredator){
		for(int i=0;i<neighbor.size();i++){ //seperate from other birds
			if(!neighbor[i]->isPredator){
				if((pos - neighbor[i]->pos).magnitude() < SEPERATION_RADIUS){
					vec = vec - (neighbor[i]->pos - pos)*((SEPERATION_RADIUS - (pos - neighbor[i]->pos).magnitude()));
				}
			}
			else{
				vec = vec - (neighbor[i]->pos - pos)*100000000;
			}
		}
	}
	for(int i=0;i<obstacle.size();i++){
		vec = vec - (obstacle[i]->pos - pos)*5000;
	}
	return vec;
}


PosVec Boid::alignment(){
	PosVec avSpeed(0,0);
	if(neighbor.size() == 0 || isPredator){
		return avSpeed;
	}
	for(int i=0;i<neighbor.size();i++){
		if((pos - neighbor[i]->pos).magnitude() < ALIGNMENT_RADIUS){
			avSpeed = avSpeed + neighbor[i]->speed;
		}
	}
	avSpeed = avSpeed / (neighbor.size());
	return (avSpeed - speed);	
}

PosVec Boid::cohesion(){
	int size = neighbor.size();
	if(size == 0){
		return pos;
	} 
	PosVec avPos(0,0);
	for(int i=0;i<size;i++){
		if(!neighbor[i]->isPredator){
			size -= 1;
			avPos = avPos + neighbor[i]->pos;
		}
	}
	avPos = avPos / size;					
	return (avPos - pos);
}


float Boid::setAngle(float ang){
	float newAngle = speed.angle(); 
	float outAngle = 0;
	
	if(newAngle > (360-MAX_ANGLE) && angle < MAX_ANGLE){
		outAngle = (360 + angle) - MAX_ANGLE;
	}
	else if(angle > (360-MAX_ANGLE) && newAngle < MAX_ANGLE){
		outAngle = 5 - (360 - angle);
	}
	else{
		if(fabs(newAngle - angle) > MAX_ANGLE){
			if(newAngle > angle){
				outAngle = angle + MAX_ANGLE;
			}
			else{
				outAngle = angle - MAX_ANGLE;
			}
		}
		else{
			return newAngle;
		}
	}
	return outAngle;
}


//----------------------------------------
//Draw and print functions
//----------------------------------------
void Boid::draw(){
	glPushMatrix();
	glTranslatef(pos.x,pos.y,0);
	glRotatef(angle,0,0,1); 
	glBegin(GL_TRIANGLES);							// Drawing Using Triangles
	glVertex3f(-0.1*radius, 0.2*radius, 0);			// Back left
	glVertex3f(-0.1*radius, -0.2*radius, 0);		// Back right
	glVertex3f( 0.5*radius, 0.0, 0);				// Front
    glEnd();
	glPopMatrix();
}	


void Boid::draw_obst(){
	glPushMatrix();
	glTranslatef(pos.x,pos.y,0);
	glBegin(GL_POLYGON);
    float x = (float)radius * cos(359 * PI/180.0f);
    float y = (float)radius * sin(359 * PI/180.0f);
    for(int j = 0; j < 360; j++){
        glVertex3f(x,y,0);
        x = (float)radius * cos(j * PI/180.0f);
        y = (float)radius * sin(j * PI/180.0f);
        glVertex3f(x,y,0);
    }
	glEnd();
	glPopMatrix();
}	
