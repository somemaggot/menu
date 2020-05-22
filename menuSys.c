#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "menuSys.h"
#include "menuStruct.h"
#include <stdio.h>

static node_t* menu = NULL;
static int node_ind = 0;
static void (*_handler)(char*);
static int _win;

void _keyboardFunc(unsigned char key, int x, int y);
void _specialFunc(int key, int x, int y);
void _reshapeFunc(int w, int h);
void _displayFunc(void);
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
	node_t* temp = menu;
	switch (key){
		case 13:
			(*_handler)(menuUpdate(&menu, node_ind));
			if (temp != menu){
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

void _displayFunc(void){
	menuDisplay(menu, node_ind);
}