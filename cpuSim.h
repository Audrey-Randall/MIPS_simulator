/*
Any function whose return value is a meta error code is of type int, anything returning a useful value is a t type
What should we do about the ALU control?
We don't want a separate variable for the pc, do we?
*/

#ifndef CPUSIM
#define CPUSIM

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "registers.h"
#include "control.h"

#define MEMSIZE 4096

typedef struct Instruction{
  uint8_t opcode:6;
  uint8_t rs:5;
  uint8_t rt:5;
  uint8_t rd:5;
  int8_t shamt:5;
  uint8_t funct:6;
  int16_t imm;
  uint32_t addr:26;
}Instruction;

uint32_t memory[MEMSIZE];    //Program memory
Instruction curIns;


//Functions
void init();                //Initializes everything
int setCurIns();
int32_t ALU(int x, int y);
int32_t signExt(int32_t offsetField);
int dataMemoryUnit(int32_t addr, int32_t writeData);
int32_t mux(int32_t zero, int32_t one, uint8_t ctrl);

void FetchStage();
void DecodeStage();
void ExecuteStage();
void MemoryStage();
void WritebackStage();

int textFileConversion(FILE *fp, int * instructionMemory);



#endif
