#ifndef CONTROL
#define CONTROL

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "cpuSim.h"


typedef enum I_Tracker {Rtype = 0, Itype = 1, Jtype = 2} Instruction_Tracker;

Instruction_Tracker InstructionType;

typedef struct ControlUnit {
  uint8_t ALUop:2;
  uint8_t ALUsrc:1;
  uint8_t RegWrite:1;
  uint8_t MemWrite:1;
  uint8_t MemToReg:1;
  uint8_t MemRead:1;
  uint8_t PCsrc:1;
  uint8_t RegDst:1;
  uint8_t Branch:1;
}ControlUnit;

ControlUnit controlUnit;
uint32_t idx; //global to keep track of actual size of file
int setControls(uint8_t opcode);
int textFileConversion(FILE *fp);

#endif
