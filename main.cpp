#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
using namespace std;

int request[9] = { 0 }, counter[3] = { 0 }, pages[3] = { -1,-1,-1 }, fault[9] = { 0 }, colour[9] = { 1,1,1,1,1,1,1,1,1 }, pagecolour[3] = { 1,1,1 };
float assign[9] = { -5.5,-5.5,-5.5,-5.5,-5.5,-5.5,-5.5,-5.5,-5.5 };
int dest = 0, showresult = 0;
int flag[3] = { 0,0,0 };
int step = -1, startani = 0, faults = 0;
char res[] = "Total page faults =  ";
float bgcolor[][3] = { { 1,0,0 },{ 0,1,0 },{ 0,0,1 } };                 //To choose background color from menu 
int bgpointer = 0;
float tilecolor[][3] = { { 1,1,0 },{ 1,0.7,0.7 },{ 0,1,1 },{1,0,1} };	//To choose tile and flag color from menu
int tilepointer = 0;

void init(){							//Initializing the orthogonal space
	glColor3f(0, 0, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 800, 0, 800);
	glMatrixMode(GL_MODELVIEW);
}

int getLRU(){						     //Returns the memory frame number that is least recently used
	if (counter[0] >= counter[1] && counter[0] >= counter[2]) return 0;
	if (counter[1] >= counter[0] && counter[1] >= counter[2]) return 1;
	return 2;
}

void tile(float x, float y, char n){			//Draws the tiles representing page requests
	float size = 20;
    //glColor3f(1,1,0);
    glBegin(GL_POLYGON);
	glVertex2f(x - size, y - size);
	glVertex2f(x + size, y - size);
	glVertex2f(x + size, y + size);
	glVertex2f(x - size, y + size);
	glEnd();
	glColor3f(0, 0, 0);
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);						//Gives each tile a border
    glVertex2f(x - size, y - size);
	glVertex2f(x + size, y - size);
	glVertex2f(x + size, y + size);
	glVertex2f(x - size, y + size);
	glEnd();
	glRasterPos2f(x - size / 2, y);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, n);	//To draw the page number over the tile
}

void drawFlag(){						//Draws a flag in the shape of a small square whenever page fault occurs
	glColor3fv(bgcolor[bgpointer]);
	glBegin(GL_POLYGON);
	glVertex2f(-10, -10);
	glVertex2f(10, -10);
	glVertex2f(10, 10);
	glVertex2f(-10, 10);
	glEnd();
}

void setupRequest(){		//Sets up the requested memory pages
	glColor3fv(bgcolor[bgpointer]);
	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(700, 0);
	glVertex2f(700, 100);
	glVertex2f(0, 100);
	glEnd();

	glPushMatrix();
	glTranslatef(-10, 40, 0);
	for(int i = 0; i < 9; i++){			//A loop to set up the 9 tiles with page numbers on them
		glColor3fv(tilecolor[tilepointer]);
		glTranslatef(70, 0, 0);
		glPushMatrix();

		if(assign[i] > -4.5){			//Checks if the tile is within the tab of requests or not
			glTranslatef(70 * step - 70 * i, 0, 0);
		}

		glTranslatef(0, -250 - 45 * assign[i], 0);		//Begins translation of the tile from original position to destination
		
		if((int)assign[i] == dest && assign[i] >= 0){
			glColor3f(0, 0, 1);      //Colours the memory frame to be replaced with blue
		}							
		else{
			glColor3fv(tilecolor[tilepointer]);
		}

		tile(10, 10, request[i] + '0');		//Calls the tile function to draw each tile
		glPopMatrix();

		if (fault[i]){					//Checks if a page fault has occurred at the time of processing the particular page request or not
			glPushMatrix();
			glTranslatef(0, -385, 0);
			drawFlag();				//Draws a flag calling the drawFlag function
			glPopMatrix();

		}
	}

	glPopMatrix();
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
    cout << "Enter a sequence of 9 numbers for page request\n";   //Asking for user input of page requests

	for (int i = 0; i < 9; i++){
        cin >> request[i];
    }

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Page Replacement");
    glClearColor(0.0, 0.0, 0.0, 0.0);   //Sets background colour of screen

    gluOrtho2D(0, 800, 0, 800);			//Defines orthogonal space
    glutMainLoop();

    return 0;
}
