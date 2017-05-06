#ifndef CONTROL
#define CONTROL

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "cpuSim.h"


//R-formats
#define OPCODE_R 0b000000 //0x00

//I-formats
#define OPCODE_ADDI 0x8
#define OPCODE_ADDUI 0x9
#define OPCODE_ANDI 0xc
#define OPCODE_XORI 0b001110
#define OPCODE_ORI 0b1110
#define OPCODE_SLTI 0xa
#define OPCODE_SLTUI 0xb
//#define OPCODE_SEB //yeah


//Branches
#define OPCODE_BEQ 0b000100 //0x4
#define OPCODE_BNE 0x5
#define OPCODE_BGTZ 7
#define OPCODE_BLTZ 1
#define OPCODE_BLEZ 6

//loads
#define OPCODE_LB 0b100000
#define OPCODE_LBU 0x24
#define OPCODE_LHU 0x25
#define OPCODE_LUI 0xf
#define OPCODE_LW 0b100011 //0x23

//stores
#define OPCODE_SB 0x28
#define OPCODE_SH 0x29
#define OPCODE_SW 0b101011 //0x2b

//jumps
#define OPCODE_J 0x2
#define OPCODE_JAL 0x3


//nop
#define OPCODE_NOP -1


//define types
#define Rform 0
#define Loads 1
#define stores 2
#define branches 3
#define jumps 4
#define Iform 5

uint8_t type;
int BranchAddr;
int amTagalong;

typedef enum I_Tracker {Rtype = 0, Itype = 1, Jtype = 2} Instruction_Tracker;

Instruction_Tracker InstructionType;

typedef struct ControlUnit {
  uint8_t ALUop;
  uint8_t ALUsrc;
  uint8_t RegWrite;
  uint8_t MemWrite;
  uint8_t MemToReg;
  uint8_t MemRead;
  uint8_t PCsrc;
  uint8_t RegDst;
  uint8_t Branch;
  uint8_t Jump;
}ControlUnit;

typedef struct pipelinedControl {
    uint8_t fetch;
    uint8_t decode;
    uint8_t execute;
    uint8_t memory;
    uint8_t writeback;
} pipelineControl;

pipelineControl pipeCtl;

ControlUnit controlUnit;
uint32_t idx; //global to keep track of actual size of file
int setControls(uint16_t opcode);
int textFileConversion(FILE *fp);

#endif
