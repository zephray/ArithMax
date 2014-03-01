#ifndef __FILEGUI_H
#define __FILEGUI_H
#include "sysbase.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "fileProvider.hpp"

int fileBrowser(char* filename, unsigned char* filter, char* title);
int fileBrowserSub(char* browserbasepath, char* filename, unsigned char* filter, char* title);

#endif