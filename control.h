#ifndef CONTROL
#define CONTROL

#include <stdint.h>
#include <string.h>

#define OPCODE_R 0b000000
#define OPCODE_LW 0b100011
#define OPCODE_SW 0b101011
#define OPCODE_BEQ 0b000100

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

#endif
