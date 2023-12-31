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

void drawText(const char *string, float x, float y, float z){ //Function to display any wordings on the screen at the given co-ordinates
	const char *c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++){
		if (*c == '\n')
			glRasterPos3f(x, y - 0.05, z);
		else
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);	//draws in the mentioned font type
	}
}

void setup_pages(){				//Sets up the memory frames with the page requests it contains at any point of time
	glPushMatrix();
	tile(0, 0, pages[0] == 0 ? ' ' : pages[0] + '0');
	glTranslatef(0, -45, 0);
	tile(0, 0, pages[1] == 0 ? ' ' : pages[1] + '0');
	glTranslatef(0, -45, 0);
	tile(0, 0, pages[2] == 0 ? ' ' : pages[2] + '0');
	glPopMatrix();
}





void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(120 + 70 * step, 200, 0);	//Translates the set of memory frames based on the request being processed
	setup_pages();							//Calls the function to draw the memory frames
	glPopMatrix();
	glColor3f(1, 1, 0);
	glPushMatrix();
	glTranslatef(50, 400, 0);
	setupRequest();					//Calls the function to draw the page requests
	glPopMatrix();
	glEnd();
	if (showresult && step == 8){		//Checks if all page requests have been processed
		glColor3f(1, 1, 1);
		res[21] = faults + '0';
		drawText(res, 50, 20, 0);		//Displays the total number of faults that have occurred
	}
	drawText("VISUALIZING LRU / FIFO PAGE REPLACEMENT", 200, 550, 0);
	drawText("(Press f/F for FIFO and Left Click for LRU)", 220, 520, 0);
	glFlush();
	glutSwapBuffers();
}



void idle(){
	if (!startani)
		return;
	if (dest > assign[step])
		assign[step] += 0.01;		//Keeps incrementing the assign of step until it equals destination
	if (dest <= assign[step]){
		if (fault[step])
			pages[dest] = request[step];   //places the requested page in the corresponding memory frame
		startani = 0;
		dest = -10;
		showresult = 1;
	}
	display();
}





void mouse(int btn, int state, int x, int y){		//mouse event callback to implement LRU algorithm
	int n, i, j;
	if (startani == 1)
		return;
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN && step < 8){
		if (step < 9)
			step++;
		for (i = 0; i < 3; i++){
			if (request[step] == pages[i]){			//Checks if the requested page is already in memory or not
				fault[step] = 0;					//No pagefault
				counter[i] = 0;						//Makes counter of the frame 0
				for (j = 0; j < 3; j++)
					if (j != i) counter[j]++;		//Increments the counters of rest of the pages
				dest = -5;
				startani = 1;
				colour[step] = 0;
				glutPostRedisplay();				//Calls display function to redraw the changes
				return;
			}
		}
		n = getLRU();							//Implies page isnt in memory and fetches the frame to be replaced with the requested page
		counter[n] = 0;
		for (i = 0; i < 3; i++)
			if (i != n)
				counter[i]++;
		dest = n;
		startani = 1;
		fault[step] = 1;
		faults++;							//Implies a page fault
	}
	glutPostRedisplay();					//Calls display function to redraw the changes
}

int getFIFO() {
    if (step == 0){
        flag[0] = 1;
        return 0;
    }
	else{
        if (flag[0] == 1){
            flag[0] = 0;
            flag[1] = 1;
            return 1;
        }
		else if (flag[1] == 1){
            flag[1] = 0;
            flag[2] = 1;
            return 2;
        }
		else if (flag[2] == 1){
            flag[2] = 0;
            flag[0] = 1;
            return 0;
        }
    }
    return 0;
}

void mykey(unsigned char key, int x, int y){			//A keyboard event callback to implement LRU algorithm
	int n, i, j;
	if (startani == 1)
		return;
	if ((key == 'f' || key == 'F') && step < 8){
		if (step < 9)
			step++;
		for (i = 0; i < 3; i++){
			if (request[step] == pages[i]){		//Checks if the requested page is already in memory or not
				fault[step] = 0;				//No pagefault
				dest = -5;
				startani = 1;
				colour[step] = 0;
				glutPostRedisplay();
				return;
			}
		}
		n = getFIFO();							//Implies page isnt in memory and fetches the frame to be replaced
		dest = n;
		startani = 1;
		fault[step] = 1;
		faults++;
	}
	glutPostRedisplay();						//Calls display function to redraw the changes
}

void handle_bg_colour(int action){			//Submenu for changing background colour{
	bgpointer = action;
	glutPostRedisplay();
}

void handle_tile_colour(int action){			//Submenu for changing tile and flag colour
	tilepointer = action;
	glutPostRedisplay();
}

void menu(int action){
	if (action == 0)
		exit(0);
}

void addMenu(){				//Menu and submenu implementation
	int submenu1, submenu2;
	submenu1 = glutCreateMenu(handle_bg_colour);			//Submenu for changing background colour of panel
	glutAddMenuEntry("Red", 0);
	glutAddMenuEntry("Green", 1);
	glutAddMenuEntry("Blue", 2);
	submenu2 = glutCreateMenu(handle_tile_colour);			//Submenu for changing tile and flag colour
	glutAddMenuEntry("Yellow", 0);
	glutAddMenuEntry("Light Red", 1);
	glutAddMenuEntry("Light Blue", 2);
	glutAddMenuEntry("Pink", 3);
	glutCreateMenu(menu);
	glutAddSubMenu("Background Colour", submenu1);
	glutAddSubMenu("Tile Colour", submenu2);
	glutAddMenuEntry("Quit", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
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
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutKeyboardFunc(mykey);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);
	glutKeyboardFunc(mykey);
	glutIdleFunc(idle);
	glEnable(GL_LIGHT3);
	init();
	addMenu();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluOrtho2D(0, 800, 0, 800);			//Defines orthogonal space
	glMatrixMode(GL_MODELVIEW);
    glutMainLoop();
    return 0;
}
