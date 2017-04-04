#ifndef CONTROL
#define CONTROL

#include <stdint.h>
#include <string.h>
#include <cpuSim.h>

#define OPCODE_R 0b000000 //0x00
#define OPCODE_LW 0b100011 //0x23
#define OPCODE_SW 0b101011 //0x2b
#define OPCODE_BEQ 0b000100 //0x4

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
int setControls(uint8_t opcode);
int setCurIns();
int textFileConversion(FILE *fp);

#endif
