/*
Any function whose return value is a meta error code is of type int, anything returning a useful value is a t type
What should we do about the ALU control?
We don't want a separate variable for the pc, do we?
*/

#ifndef CPUSIM
#define CPUSIM

#include <stdio.h>
#include "registers.h"
#include "control.h"

#define MEMSIZE 4096

uint32_t memory[MEMSIZE];    //Program memory

//Functions
void init();                //Initializes everything
int parse(char* file);      //Reads instructions from input file and stores them in memory array
int32_t ALU(int x, int y);
int32_t signExt(int32_t offsetField);
int dataMemoryUnit(int32_t addr, int32_t writeData);
int32_t mux(int32_t zero, int32_t one, uint8_t ctrl);



#endif
