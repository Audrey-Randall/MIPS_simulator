#ifndef CPUSIM
#define CPUSIM

#include <stdio.h>
#include "registers.h"

#define MEMSIZE 4096

int32_t memory[MEMSIZE];    //Program memory
uint32_t pc;                //program counter


//Functions
void init();                //Initializes everything
int parse(char* file);      //Reads instructions from input file and stores them in memory array
#endif
