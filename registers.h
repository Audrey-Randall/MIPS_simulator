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
  uint32_t instruct;
  uint16_t opcode;
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
  int32_t regdata1; //register value 1
  int32_t regdata2; //register value 2 after mux

  //int32_t write_data; //register place to write data
  //uint8_t write_reg; //Write to this reg on load instrucs. On store instrucs, this will be -1, so the mem stage can tell whether it should load or store.

  uint8_t write_to_mem_reg; //reg number for SW
  int32_t write_to_mem_val; //value for SW

  int32_t EOP_flag; //unique to this as we figure this out after decode stage

}Idex;

typedef struct Exmem {
  //carried over from ifid
  Instruction ins;

  //exmem stuff
  //uint8_t resReg; //same as write_reg? But this gets passed from ex, not id
  int32_t ALUres; //address for the mem stage to write to, if mem stage necessary. If not, it's result for reg.
  uint8_t zero;
  uint32_t addr;
  uint32_t PC;
  //uint8_t skipMem;
  uint8_t nopFlag;

  //lw
 //memory address to fetch out
  uint8_t write_reg; //register number to be written to
  uint32_t write_addr;

  //for SW
  int32_t write_data;
  uint8_t write_to_mem_reg; //reg number for SW
  int32_t write_to_mem_val; //value for SW

  //for endofprogram
  int8_t EOP_flag;
}Exmem;

typedef struct Memwb {
  int32_t ALUres;
  int32_t read_data; //data that was just read (lw)

  uint8_t write_reg;  //register to write to (lw)
  uint32_t write_addr; //address to seek from (lw)

  int32_t write_data; //memory to write to (sw)

  uint32_t PC;
  Instruction ins;

  //uint8_t skipWB;
  uint8_t nopFlag;
}Memwb;

typedef struct Registerfile {
    uint32_t regs[32];      //Registers, if you want to modify T0 you can use regs[T0]
    uint8_t readreg1;
    uint8_t readreg2;
    uint8_t writereg; //register number to be written to
    int32_t writeval; //value that needs to be written to register
    int32_t regdata1;
    int32_t regdata2;
    //readdata1 can be referred to as regs[readreg1] and same for others.
}Registerfile;

typedef struct ALUUnit {
    uint32_t input1;
    uint32_t input2;
    uint32_t zero;
    uint32_t ALUres;
    uint32_t ALUtomem; //memory address to store in memory
} ALU;


ALU alu;
Registerfile regfile;
Ifid IFID;
Idex IDEX;
Exmem EXMEM;
Memwb MEMWB;
uint32_t PC; //global PC to be updated at the end of the last pipeline register
#endif
