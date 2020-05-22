#pragma once


typedef struct Node{
	int width;
	struct Node* prev;
	struct Node** next;
	char* name;
}node_t;

node_t* menuCreate(char* menuPath);
void menuDestroy(node_t* menu);
void menuDisplay(node_t* menu, int node_ind);
char* menuUpdate(node_t** menu, int node_ind);