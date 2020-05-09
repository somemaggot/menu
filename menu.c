#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "menu.h"
#include "tree.h"
#include <stdio.h>

static node_t* menu = NULL;
static int node_ind = 0;
static void (*_handler)(char*);
static int _win;

void _keyboardFunc(unsigned char key, int x, int y);
void _specialFunc(int key, int x, int y);
void _displayFunc(void);
void _reshapeFunc(void);
void _emptyFunc(void){};

int systemOpen(char* windowName, int* argc, char** argv){
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE | GLUT_STENCIL);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 640);
	_win = glutCreateWindow(windowName);
	return _win;
};

node_t* systemInit(char* menuPath, void (*uHandler)(char*)){
	if (menu == NULL){
		_handler = uHandler;
		menu = menuCreate(menuPath);
		
		glutKeyboardFunc(_keyboardFunc);
		glutSpecialFunc(_specialFunc);
		glutDisplayFunc(_displayFunc);
		glutReshapeFunc(_reshapeFunc);
		return menu;
	}
	return NULL;
};

void systemRelease(void){
	if (menu != NULL){
		menuDestroy(menu);
		menu = NULL;
		node_ind = 0;
		glutKeyboardFunc(_emptyFunc);
		glutSpecialFunc(_emptyFunc);
		glutDisplayFunc(_emptyFunc);
	}
};

void _displayFunc(void){
	int i, j;
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	gluOrtho2D(0, 29, 0, 29);
	//------------1--------------
	glBegin(GL_QUADS);
	  if (node_ind == 0){
		glColor3d(1.0, 0.0, 0.0);
	  } else {
		glColor3d(1.0, 1.0, 1.0);
	  }
	  glVertex2f(28.0, 28.0);
	  glVertex2f(1.0, 28.0);
	  glVertex2f(1.0, 21.0);
	  glVertex2f(28.0, 21.0);
	  //--------------2----------------
	  if (node_ind != 0 && (node_ind != (menu->width - 1) || menu->width == 2)){
		  glColor3d(1.0, 0.0, 0.0);
	  } else {
		  glColor3d(1.0, 1.0, 1.0);
	  }
	  if (menu->width >= 2){
		  glVertex2f(28.0, 18.0);
		  glVertex2f(1.0, 18.0);
		  glVertex2f(1.0, 11.0);
		  glVertex2f(28.0, 11.0);
	  }//------------------3-------------------
	  if (menu->width >= 3){
		  if (node_ind == (menu->width - 1)){
			  glColor3d(1.0, 0.0, 0.0);
		  } else {
			  glColor3d(1.0, 1.0, 1.0);
		  }
		  glVertex2f(28.0, 8.0);
		  glVertex2f(1.0, 8.0);
		  glVertex2f(1.0, 1.0);
		  glVertex2f(28.0, 1.0);
	  }
	glEnd();
	//-------------------1--------------------
	glColor3d(0.0, 0.0, 0.0);
	if (node_ind == 0){
		i = 1;
		glColor3d(1.0, 1.0, 1.0);
	} else if (node_ind == (menu->width - 1) && menu->width >= 3){
		i = node_ind - 1;
	} else {
		i = node_ind;
	}
	glRasterPos2f(3.0, 24.0);
	for (j = 0; menu->next[i - 1]->name[j]; j++){
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, menu->next[i - 1]->name[j]);
	}//--------------------------2----------------------
	glColor3d(0.0, 0.0, 0.0);
	if (menu->width >= 2){
		if ((node_ind < menu->width - 1 && node_ind > 0) || (node_ind == menu->width - 1 && menu->width == 2)){
			glColor3d(1.0, 1.0, 1.0);
		}
		glRasterPos2f(3.0, 14.0);
		for (j = 0; menu->next[i]->name[j]; j++){
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, menu->next[i]->name[j]);
		}//----------------------3---------------------
		if (menu->width >= 3){
			if (node_ind == menu->width - 1){
				glColor3d(1.0, 1.0, 1.0);
			} else {
				glColor3d(0.0, 0.0, 0.0);
			}
			glRasterPos2f(3.0, 4.0);
			for (j = 0; menu->next[i + 1]->name[j]; j++){
				glutBitmapCharacter(GLUT_BITMAP_8_BY_13, menu->next[i + 1]->name[j]);
			}
		}
	}
	glutSwapBuffers();
}

void _reshapeFunc(int w, int h){
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glutReshapeWindow(600, 600);
	glMatrixMode(GL_MODELVIEW);
}

void _specialFunc(int key, int x, int y){
	switch (key){
		case GLUT_KEY_UP:
			if (node_ind > 0){
				node_ind--;
			}
			break;
		case GLUT_KEY_DOWN:
			if (node_ind < (menu->width - 1)){
				node_ind++;
			}
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

void _keyboardFunc(unsigned char key, int x, int y){
	switch (key){
		case 13:
			if (menu->next[node_ind]->width == 0){
				(*_handler)(menu->next[node_ind]->name);
			} else {
				menu = menu->next[node_ind];
				node_ind = 0;
			}
			break;
		case 27:
			if (menu->prev != NULL){
				menu = menu->prev;
				node_ind = 0;
			} else {
				while (menu->prev != NULL);
				menuDestroy(menu);
				exit(0);
			}
			break;
	}
	glutPostRedisplay();
}
