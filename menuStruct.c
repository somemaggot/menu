#include <stdio.h>
#include "menuStruct.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

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
