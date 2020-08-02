#include "Main.h"

//Local variables
vector<Boid*> birds;
vector<Boid*> predators;
vector<Boid*> obstacles;
int frameCount=0;
int currentTime,previousTime=glutGet(GLUT_ELAPSED_TIME);
simulation_state simMode = BIRDS;
float fps;
bool pauseState=false;
float sepWeight, alignWeight, cohWeight;


//Local functions
void calculateFPS(void);
void printFPS(void);
void printForces(void);						
void drawBanner(void);										//Draw banner around birds
void drawMode(void);										//Draw current mode
void display(void);											//callback for updating grafic
void idle(void);											//callback for idle state
void pause(void);											//callback for paused state
void key(unsigned char k, int x, int y);					//callback for key pressed event
void reshape(int width, int height);						//callback for window resize
void clear_boids();											//delete all boids
void updateBirds();											//update bird neighbors, speed and position
void updateBirdsAndPredators();								//update bird and predator neighbors, speed and position
void updateObstacles(bool isPred);							//update bird and predator obstacles
void init_openGL(void);										//initiate openGL settings
void init_boids(int birdsNum, int predNum);					//initiate birds and predators
void init_obstacles(void);									//initiate obstacles (in boids class)



int main (int argc, char* argv[]){
	//init_openGL();
	clear_boids();
	init_boids(BOIDS_NUMBER, PRED_NUMBER); //number of birds
	init_obstacles(); 

	sepWeight = 0.5;
	alignWeight = 10;
	cohWeight = 6;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); //RGB display | depth testing | double buffering enabled
	glutInitWindowSize(GRIDSIZE,GRIDSIZE);
	glutCreateWindow("IT3708 - Boids");
	
	//callback functions
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key); //changes simMode with key input
	glutIdleFunc(idle); //background functions when no other input
	glutDisplayFunc(display);

	glutMainLoop();
}


//------------------------
//CALCULATE FUNCTIONS
//------------------------
void calculateFPS(){
    frameCount++;
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    int timeInterval = currentTime - previousTime;

    if(timeInterval > 1000){
		fps=frameCount / (timeInterval / 1000.0f);
        previousTime = currentTime;

        frameCount = 0;
    }
}


//------------------------
//PRINT AND DRAW FUNCTIONS
//------------------------
void printFPS(){
	glColor3f((255-fps)/255,fps/255,0);
	char value[20];
	sprintf(value,"FPS:%.0f",fps);
	text(10,20,0,value,2).print();
}

void printForces(){
	char value[32];
	glColor3f(1,1,sepWeight);
	sprintf(value,"Separation force:%.3f",sepWeight);
	text(5,-18,0,value,1).print();
	glColor3f(1,1,alignWeight);
	sprintf(value,"Alignment force:%.3f",alignWeight);
	text(5,-19,0,value,1).print();
	glColor3f(1,1,cohWeight);
	sprintf(value,"Cohesian force:%.3f",cohWeight);
	text(5,-20,0,value,1).print();
}

void drawBanner(){
	glColor3f(0.0, 0.8, 0.2); //green instructions
	text(-20,-16,0,"SPACE - Change simulation",1).print();
	text(-20,-17,0,"p     - Pause simulations",1).print();
	text(-20,-18,0,"q/a   - Adjust separation force",1).print();
	text(-20,-19,0,"w/s   - Adjust alignment force",1).print();
	text(-20,-20,0,"e/d   - Adjust cohesian force",1).print();
	printFPS();
	printForces();
}

void drawMode(){
	glTranslatef(0.0,0.0,-50.0);
    
	drawBanner();
	glColor3f(0.8, 0.6, 0.2); //orange birds
	for(int i=0;i<birds.size();i++) birds[i]->draw();
	glColor3f(0.0, 0.6, 0.8); //blue header writing

	switch(simMode){
		case BIRDS:
			text(-17,20,0,"Birds",2).print();
			break;
		case BIRDS_OBST:
			text(-17,20,0,"Birds and obstacles",2).print();
			glColor3f(0.4, 0.0, 0.8); //purple obstacles
			for(int i=0;i<obstacles.size();i++) obstacles[i]->draw_obst();
			break;
		case BIRDS_PRED:
			text(-17,20,0,"Birds and predators",2).print();	
			glColor3f(1.0,0,0); //red predators
			for(int i=0;i<predators.size();++i) predators[i]->draw();
			break;
		case BIRDS_OBST_PRED:
			text(-17,20,0,"Birds, obstacles and predators",2).print();
			glColor3f(0.4, 0.0, 0.8); //purple obstacles
			for(int i=0;i<obstacles.size();i++) obstacles[i]->draw_obst();
			glColor3f(1.0,0,0); //red predators
			for(int i=0;i<predators.size();++i) predators[i]->draw();
			break;
	}
}

//------------------------
//GLUT CALLBACK FUNCTIONS
//------------------------

//Update graphic
void display(void){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //clear all drawings
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity(); //initialise the modelview matrix to identity matrix
	
	drawMode();
	
	glFlush(); //flush what we've drawn to the buffer
	glutSwapBuffers(); //send to screen
}


//called when nothing else being done
void idle(void){ 
	switch(simMode){
		case BIRDS:
			updateObstacles(false);
			updateBirds();
			break;
		case BIRDS_OBST:
			updateObstacles(true);
			updateBirds();
			break;
		case BIRDS_PRED:
			updateObstacles(false);
			updateBirds();
			updateBirdsAndPredators();
			break;
		case BIRDS_OBST_PRED:
			updateObstacles(true);
			updateBirds();
			updateBirdsAndPredators();
			break;
		}
	calculateFPS();
	glutPostRedisplay();
}


//called when in paused mode
void pause(void){
	calculateFPS();
	glutPostRedisplay();
}

//called when key pressed
void key(unsigned char k, int x, int y){ 
	switch(k){
		case 27: //ASCII: ESCAPE
			exit(0);
			break;
		case 'p':
			printf("Pause state: %i\n",pauseState);
			if(pauseState==false){
				pauseState=true;
				glutIdleFunc(pause);
			} 
			else{
				pauseState=false;
				glutIdleFunc(idle);
			}
			break;
		case 32: //ASCII: SPACE
			switch(simMode){
				case BIRDS: simMode = BIRDS_OBST; break;
				case BIRDS_OBST: simMode = BIRDS_PRED; break;
				case BIRDS_PRED: simMode = BIRDS_OBST_PRED; break;
				case BIRDS_OBST_PRED: simMode = BIRDS; break;
			}
			break;
		case 'q':
			sepWeight+=0.01;
			break;
		case 'a':
			sepWeight-=0.01;
			break;
		case 'w':
			alignWeight+=0.01;
			break;
		case 's':
			alignWeight-=0.01;
			break;
		case 'e':
			cohWeight+=0.01;
			break;
		case 'd':
			cohWeight-=0.01;
			break;
	}
}

//called when window size changed
void reshape(int width, int height){
	glViewport(0,0,width, height-10); //set viewport to same width and height as window
	glMatrixMode(GL_PROJECTION); //make changes to the projection matrix
	glLoadIdentity();
	gluPerspective(50.0, (float) width / (float) (height+80), 1.0, 100.0); //projection type: perspective projection with 90 degree field of view
	glMatrixMode(GL_MODELVIEW); //make changes to the modelview matrix
	glLoadIdentity();
}


//------------------------
//Update functions
//------------------------
void updateBirds(){
	for(int i=0;i<birds.size();i++) {
		birds[i] = birds[i]->updateBoidNeighbors(birds);
	}
	for(int i=0;i<birds.size();i++) {
		birds[i] = birds[i]->move(sepWeight, alignWeight, cohWeight);
	}
}


void updateBirdsAndPredators(){
	for(int i=0;i<birds.size();i++) {
		birds[i] = birds[i]->updateBoidNeighbors(birds, predators);
	}
	for(int i=0;i<predators.size();i++) {
		predators[i] = predators[i]->updateBoidNeighbors(birds, predators);
	}
	for(int i=0;i<birds.size();i++) {
		birds[i] = birds[i]->move(sepWeight, alignWeight, cohWeight);
	}
	for(int i=0;i<predators.size();i++) {
		predators[i] = predators[i]->move(sepWeight, alignWeight, cohWeight);
	}
}


void updateObstacles(bool useObstacle){
	for(int i=0;i<birds.size();i++) {
		birds[i] =  birds[i]->updateObstacles(obstacles, useObstacle);
	}
	for(int i=0;i<predators.size();i++) {
		predators[i] = predators[i]->updateObstacles(obstacles, useObstacle);
	}
}


//------------------------
//Init functions
//------------------------
void init_openGL(void){
	glShadeModel(GL_SMOOTH); //GL_FLAT = flat shading
	glDisable(GL_LIGHTING); //turn lighting off
	glDepthFunc(GL_LEQUAL); //enable OpenGL hidden surface removal
	glEnable(GL_DEPTH_TEST);
}

void clear_boids(){
	while(birds.size()>0){
		delete birds[0];
		birds.erase(birds.begin());
	}
	while(predators.size()>0){
		delete predators[0];
		predators.erase(predators.begin());
	}
}

void init_boids(int boidsNum, int predNum){
	clear_boids();
	Boid *nBoid;
	int num = floor(sqrt(boidsNum));
	for(int i=1;i<=num;i++){ //number of boids
		for(int j=1;j<=num;j++){
			nBoid=new Boid(PosVec(i,j),PosVec(1,0),1,false);
			birds.push_back(nBoid);
		}
	}
	nBoid=new Boid(PosVec(12,0),PosVec(UNIT_SPEED,0),2,true);
	predators.push_back(nBoid);
}

void init_obstacles(){
	Boid *obst;
	obst=new Boid(PosVec(10,-3),PosVec(0,0),2,true);
	obstacles.push_back(obst);
	obst=new Boid(PosVec(-8,5),PosVec(0,0),4,true);
	obstacles.push_back(obst);
}