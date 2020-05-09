#pragma once


typedef struct Node{
	int width;
	struct Node* prev;
	struct Node** next;
	char* name;
}node_t;

node_t* menuCreate(char* menuPath);
void menuDestroy(node_t* menu);