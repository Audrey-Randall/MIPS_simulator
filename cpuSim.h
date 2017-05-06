/*
Any function whose return value is a meta error code is of type int, anything returning a useful value is a t type
What should we do about the ALU control?
We don't want a separate variable for the pc, do we?
*/

#ifndef CPUSIM
#define CPUSIM

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "registers.h"
#include "control.h"
#include "memory.h"
//Instruction curIns; //should be replaced by pipeline registers


//Functions
void init();                //Initializes everything
int parseInput();           //Reads input file and fills memory array with it
int setCurIns();
int32_t ALUfunct(int32_t v1, int32_t v2, uint8_t * err_p, uint8_t resReg);

int32_t signExt(int16_t offsetField);
int dataMemoryUnit(int32_t addr, int32_t writeData);
int32_t mux(int32_t zero, int32_t one, uint8_t ctrl);
void store();
int32_t load();

uint32_t tinstructioncount;

#endif
