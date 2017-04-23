#ifndef REGISTERS
#define REGISTERS
#include <stdint.h>
#include "cpuSim.h"


#define ZERO 0
#define AT 1
#define V0 2
#define V1 3
#define A0 4
#define A1 5
#define A2 6
#define A3 7
#define T0 8
#define T1 9
#define T2 10
#define T3 11
#define T4 12
#define T5 13
#define T6 14
#define T7 15
#define S0 16
#define S1 17
#define S2 18
#define S3 19
#define S4 20
#define S5 21
#define S6 22
#define S7 23
#define T8 24
#define T9 25
#define K0 26
#define K1 27
#define GP 28
#define SP 29
#define FP 30
#define RA 31

typedef struct Instruction{
  uint8_t opcode:6;
  uint8_t rs:5;
  uint8_t rt:5;
  uint8_t rd:5;
  int8_t shamt:5;
  uint8_t funct:6;
  int16_t imm;
  int32_t signextimm;
  uint32_t addr:26;
}Instruction;

typedef struct Ifid{
  Instruction ins;
  uint32_t PC;
  uint8_t nopFlag; //Don't do anything else for the rest of the instruc if this is set
}Ifid;

typedef struct Idex {
  Instruction ins;
  uint32_t PC;
  uint8_t nopFlag;
  int32_t write_data;
  int8_t write_reg; //Write to this reg on load instrucs. On store instrucs, this will be -1, so the mem stage can tell whether it should load or store.
}Idex;

typedef struct Exmem {
  uint8_t resReg; //same as write_reg? But this gets passed from ex, not id
  int32_t ALUres; //address for the mem stage to write to, if mem stage necessary. If not, it's result for reg.
  uint8_t zero;
  int32_t write_data; //Passed through from decode stage
  int8_t write_reg; //Passed through from decode
  uint32_t addr;
  uint32_t PC;
  uint8_t skipMem;
  uint8_t nopFlag;
}Exmem;

typedef struct Memwb {
  uint8_t resReg; //same as write_reg?
  uint32_t ALUres;
  int32_t read_data; //data that was just read in
  int32_t write_data; //Data to be written
  int8_t write_reg; //If -1, write the write_data to ALUres. If valid reg number, read datamem[ALUres] into regs[write_reg]
  uint32_t PC;
  uint8_t skipWB;
  uint8_t nopFlag;
}Memwb;

uint32_t regs[32];      //Registers, if you want to modify T0 you can use regs[T0]
Ifid IFID;
Idex IDEX;
Exmem EXMEM;
Memwb MEMWB;
uint32_t PC;
#endif
