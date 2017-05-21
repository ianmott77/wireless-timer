#ifndef TOOLS_H
#define TOOLS_H

#include <Arduino.h>
#include <MemoryFree.h>

char * str_append(char *, const char *);
char * substr(char *, int, int);
bool str_ends(char *, const char *);
void mem();
#endif