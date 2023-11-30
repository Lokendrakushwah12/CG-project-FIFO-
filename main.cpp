#include<iostream>
#include<GL/glut.h>
using namespace std;

int request[9] = { 0 }, counter[3] = { 0 }, pages[3] = { -1,-1,-1 }, fault[9] = { 0 }, colour[9] = { 1,1,1,1,1,1,1,1,1 }, pagecolour[3] = { 1,1,1 };

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