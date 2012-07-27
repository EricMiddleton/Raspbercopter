#ifndef MEMORY_H
#define MEMORY_H
#include <Arduino.h>

int freeListSize();
int CheckMem();

extern unsigned int __heap_start;
extern void *__brkval;

#endif