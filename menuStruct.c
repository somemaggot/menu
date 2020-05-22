#include <stdio.h>
#include "menuStruct.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int _match(char* str, char* substr){
	int i = 0;
	int retval = 1;
	while (str[i] && substr[i] && retval){
		if (substr[i] != str[i]){
			retval = 0;
		}
		i++;
	}
	if (substr[i]){
		retval = 0;
	}
	return retval;
}

void menuDisplay(node_t* menu, int node_ind){
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

node_t* menuCreate(char* menuPath){
	//menu is a tree, function returns a first-level node, windows are branches and buttons are leaves
	FILE* file = fopen(menuPath, "r");
	int strlen = 30;
	char* str;
	char buff[10];
	char* char_p;
	int i = 0;
	int depth = 0;
	node_t* menu = (node_t*)malloc(sizeof(node_t));
	enum state{
		WINDOW,
		BUTTON,
		ERR,
	}state = WINDOW;
	int isTooShort = 1;

	menu->prev = NULL;
	menu->width = 0;

	str = (char*)malloc(strlen * sizeof(char));
	char_p = str;
	while (fgets(str, strlen - 1, file) && state != ERR){
		while(isspace(str[i])){
			char_p++;
			i++;
		}
		while(str[i]){
			if (str[i] == '\n'){
				isTooShort = 0;
			}
			i++;
		}
		while (isTooShort){
			strlen += 10;
			str = realloc(str, strlen);
			str[strlen] = 0;
			if(fgets(buff, 11, file)){
				strcat(str, buff);
				while(str[i]){
					if (str[i] == '\n'){
						isTooShort = 1;
					}
					i++;
				}
			} else {
				isTooShort = 1;
			}
		}
		i = 0;
			if (_match(char_p,  "WindowBegin") || _match(char_p, "ButtonBegin")){
				if (state == BUTTON){
					//whether window or button, both created in other window, not button
					state = ERR;
				} else {
					if (!menu->width){
						menu->width++;
						menu->next = (node_t**)malloc(2*sizeof(node_t*));
						menu->next[0] = (node_t*)malloc(sizeof(node_t));
						menu->next[1] = 0;
						menu->next[0]->prev = menu;
						menu = menu->next[0];
						menu->width = 0;
					} else {
						menu->next = realloc(menu->next, (menu->width + 1)*sizeof(node_t*));
						menu->next[menu->width] = (node_t*)malloc(sizeof(node_t));
						menu->next[menu->width + 1] = 0;
						menu->next[menu->width]->prev = menu;
						menu->width++;
						menu = menu->next[menu->width - 1];
					}
					if (menu->next != NULL && menu->name != NULL){
						
					}
					depth++;
				}
				menu->name = NULL;
				menu->next = NULL;
				menu->width = 0;
				if(_match(char_p, "WindowBegin")){
					state = WINDOW;
				} else {
					state = BUTTON;
				}
			} else if (_match(char_p, "WindowEnd")){
				if (state == BUTTON || menu->name == NULL || menu->next == NULL){
					state = ERR;
				} else {
					if (menu->prev != NULL || depth == 0){
						menu = menu->prev;
						depth--;
					} else {
						state = ERR;
					};
				}
			} else if (_match(char_p, "ButtonEnd")){
				if (state != BUTTON || menu->name == NULL){
					state = ERR;
				} else {
					if (menu->prev != NULL){
						menu->next = NULL;
						menu = menu->prev;
						depth--;
						state = WINDOW;
					} else {
						state = ERR;
					}
				}
			} else if (_match(char_p, "Name = ")){
				char_p += 7;
				if (*char_p){
					i = 0;
					while (char_p[i]){
						i++;
					};
					if (char_p[i - 1] == '\n'){
						i--;
					}
					menu->name = (char*)malloc((i + 1)*sizeof(char));
					menu->name[i] = 0;
					while(i){
						i--;
						menu->name[i] = char_p[i];
					}
				}
			}
		char_p = str;
	}
	if (state != ERR){
		return menu;
	} else {
		return NULL;
	}
}

char* menuUpdate(node_t** menu, int node_ind){
	char* retval = (*menu)->next[node_ind]->name;
	if ((*menu)->next[node_ind]->width){
		(*menu) = (*menu)->next[node_ind];
	}
	return retval;
}

void menuDestroy(node_t* menu){
	int i;
	if (menu->next != NULL){
		for (i = 0; i < menu->width; i++){
			menuDestroy(menu->next[i]);
		}
	}
	free(menu->name);
	free(menu);
}
