#pragma once
#include "menuStruct.h"

int systemOpen(char* windowName, int* argc, char** argv);

node_t* systemInit(char* menuPath, void (*uHandler)(char*));
void systemRelease(void);


