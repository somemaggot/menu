#include "menu.h"
#include "string.h"
#include "stdio.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void handler(char*);
void displayFunc(void);
void keyboardFunc(unsigned char key, int x, int y);
static int win;

int main(int argc, char** argv){
	win = systemOpen("DooM Eternal", &argc, argv);
	systemInit("test.txt", handler);
	glutMainLoop();
	systemRelease();
}

void handler(char* button){
	if (!strcmp(button, "1")){
		printf("1!\n");
	} else if (!strcmp(button, "2")){
		printf("2?\n");
	} else if (!strcmp(button, "fourthbutton")){
		printf("I genuinely forgot the third, but whatever\n");
	} else if (!strcmp(button, "whatever")){
		printf("And now I forgot that this is the third\n");
	} else if (!strcmp(button, "The Action")){
		systemRelease();
		//win = glutCreateWindow("Blank Space");
		glutDisplayFunc(displayFunc);
		glutKeyboardFunc(keyboardFunc);
	} else {
		printf("Unhandled...\n");
	}
}

void keyboardFunc(unsigned char key, int x, int y){
	if (key == 27){
		//glutDestroyWindow(win);
		systemInit("test.txt", handler);
		glutPostRedisplay();
	}
}

void displayFunc(void){
	int i;
	char* str = "There could have been your display";
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	gluOrtho2D(0, 29, 0, 29);
	
	glColor3d(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	  glVertex2f(3.0, 15.0);
  	  glVertex2f(26.0, 15.0);
  	  glVertex2f(26.0, 10.0);
	  glVertex2f(3.0, 10.0);
	glEnd();
	glColor3d(0.0, 0.0, 0.0);
	glRasterPos2f(5.0, 12.0);
	for (i = 0; i < 34; i++){
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[i]);
	};
	
	glutSwapBuffers();
}