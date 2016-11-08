#include <stdio.h>
#include <stdlib.h>
#include "basicLib.h"
#include <ctime>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

/*
SUMMARY: 

This program simulates a Robot with a Red head and Green body

With Left click the Robot can either move in the direction it is facing
or Turn in 1 of the four directions. The menu also contains addtional functionalities. 

To do this we have an array of the custom "Point2D" class which stores the points of our
grid. This class can be found in the "BasicLib.cpp" file. 

Compile the program with "make robot" on either linux or Windows Powershell

Be sure to have the GLUT library installed. 
*/

//Global Variables
int robo;		// specifies the robot's Grid position
point2D *grid;	// the Grid, made of our custom 2D point integer 
char roboD; 	// the direciton the robot was facing 
int rowSize; 	// used to go from one row to another
int size;		// size of the Grid
int squareSize;	// size of the gridsquares

//Sets the Global values
void setGlobals(int len, int height, point2D startLoc, int sq){
	int x = startLoc.x;
	int y = startLoc.y;
	int c = 0;
	squareSize = sq;
	rowSize = len + 1;
	size = (len+1)*(height+1);
	grid = (point2D *) calloc (size, sizeof(point2D));
	printf("RowSize %i\n", rowSize);
	printf("array Size %i \n", size);
	for(int i = 0; i < height + 1; i++){
		for(int j = 0; j < rowSize; j++){
			//printf("vals %i, %i\n", i, j);
			grid[c] = point2D(x + sq *j, y + sq* i);
			//printf("Grid at %i: %i, %i\n",c, grid[c].x, grid[c].y);
			c++;
		}
	}
	srand(time(0));
	robo = rand()%size;
	if (robo % rowSize == rowSize-1)
		robo = robo - 1;
	if (robo + rowSize > size)
		robo = robo - rowSize;
	roboD = 'u';
}

//draws either a vertical or horizontal line between the two points
void makeLine (point2D p1, point2D p2){
	glBegin(GL_POINTS);
		if(p1.x == p2.x){
			if (p1.y < p2.y)
				for (int i = p1.y; i < p2.y; i ++){
					glVertex2i(p1.x, i);
			}
			else
				for (int i = p2.y; i < p1.y; i ++){
					glVertex2i(p1.x, i);
			}
		}
		else if (p1.y == p2.y){
			if (p1.x < p2.x)
				for (int i = p1.x; i < p2.x; i ++){
					glVertex2i(i, p1.y);
			}
			else
				for (int i = p2.x; i < p1.x; i ++){
					glVertex2i(i, p1.y);
			}
		}
	glEnd();
}

//creates a square at point p 
void makeSquare (point2D p){
	int y1 = p.y;
	int y2 = p.y+squareSize + 1;
	point2D temp1, temp2;
	for (int x = p.x; x < p.x + squareSize + 1; x++){
		temp1 = point2D(x,y1);
		temp2 = point2D(x,y2);
		makeLine(temp1, temp2);
	}
}

//The display Method for GLUT
//Commented lines can be used for debugging. 
void display(void){
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1,1,1);
	for(int i = 0; i < size-1; i++){
		makeLine(grid[i], grid[i+1]);
		if(i + rowSize < size){
			//printf("Doing dot %i to %i\n", i, i + rowSize);
			makeLine(grid[i], grid[i + rowSize]);
		}
		
		
	}
	glColor3f(0,1,0);
	makeSquare(grid[robo]);
	//printf("Robo at %i\n", robo);
	//makeLine(point2D(20,20), point2D(20,50));
	//makeLine(point2D(20,20), point2D(50,20));
	//glColor3f(0,0,1);
	//makeSquare(grid[rowSize]);
	//makeSquare(grid[2*rowSize]);
	//makeSquare(grid[3*rowSize]);
	//makeSquare(grid[3*rowSize+1]);
	glColor3f(1,0,0);
	if(roboD == 'd')
		makeSquare(grid[robo-rowSize]);
	else if (roboD == 'u')
		makeSquare(grid[robo + rowSize]);
	else if(roboD == 'l')
		makeSquare(grid[robo - 1]);
	else if (roboD == 'r')
		makeSquare(grid[robo+1]);
	else{
		printf("Strange Direction Error");
	}
	glFlush();
}

//Menu process for GLUT 
void menuProc(int value){
	if (value == 1){
		switch (roboD){
			case 'u':
				if(robo + rowSize < size- 2* rowSize)
					robo = robo + rowSize;
				else
					printf("Robo could not move Up");
				break;
			case 'l':	//27 is the esc key
				if(robo > 0 && (robo-1)% rowSize != 0){
					robo--;
				}
				else
					printf("Robo could not move Left");
				break;
			case 'r':
				if(robo < size-1 && (robo+1)% rowSize != rowSize-2)
					robo ++;
				else
					printf("Robo could not move right");
				break;
			case 'd':
				if(robo - rowSize > rowSize)
					robo = robo - rowSize;
				else
					printf("Robo could not move down");
				break;
		}
	}
	else if (value == 2 && robo + rowSize < size - rowSize)
		roboD = 'u';
	else if (value == 3 &&(robo+1)% rowSize != rowSize-1)
		roboD = 'r';
	else if (value == 4 && (robo-1)% rowSize != rowSize-1)
		roboD = 'l';
	else if (value == 5 && robo-rowSize > 0)
		roboD = 'd';
	else if (value == 6){
		robo = rand()%size;
		roboD = 'u';
		//printf("%i, %i\n", robo, size - 2*rowSize);
		if (robo % rowSize == rowSize-1)
			robo = robo - 1;
		if (robo > size - 2*rowSize)
			robo = robo - 2*rowSize;
		//printf("%i, %i\n", robo, size - 2*rowSize);
	}
	else if (value == 7)
		robo = size/2;
	else if (value == 8)
		exit(0);
}

//Creates the menu for GLUT 
void createOurMenu(){
	int subMenu_id = glutCreateMenu(menuProc);
	glutAddMenuEntry("Face Up", 2);
	glutAddMenuEntry("Face Right", 3);
	glutAddMenuEntry("Face Left", 4);
	glutAddMenuEntry("Face Down", 5);

	int main_id = glutCreateMenu(menuProc);
	glutAddMenuEntry("Move", 1);
	glutAddSubMenu("Turn", subMenu_id);
	glutAddMenuEntry("Shuffle", 6);
	glutAddMenuEntry("Reset", 7);
	glutAddMenuEntry("Quit", 8);
	glutAttachMenu(GLUT_LEFT_BUTTON);
}

//Used to refresh the frame for the buffer. 
void FPS(int val){
	glutPostRedisplay();
	glutTimerFunc(17, FPS, 0); // 1sec = 1000, 60fps = 1000/60 = ~17
}

/* main function - program entry point */
int main(int argc, char** argv)
{
	glutInit(&argc, argv);		//starts up GLUT
	int len = 10;
	int height = 10;
	int sq = 35;
	point2D loc = point2D(20,20);
	setGlobals(len, height, loc, sq);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(10,10);
	glutCreateWindow("Robot");	//creates the window
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 400, 0 , 400);
	glPointSize(2);
	
	glutDisplayFunc(display);	//registers "display" as the display callback
	glutTimerFunc(0, FPS, 0);
	createOurMenu();
	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}