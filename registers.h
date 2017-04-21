#ifndef REGISTERS
#define REGISTERS
#include <stdint.h>


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

typedef struct Ifid{
  Instruction ins;
  uint32_t PC;
}Ifid;

typedef struct Idex {
  Instruction ins;
  uint32_t PC;
}Idex;

typedef struct Exmem {
  int32_t res;
  uint8_t zero;
  int32_t write_data;
}Exmem;

typedef struct Memwb {
  uint32_t addr;
  int32_t read_data;
}Memwb;

uint32_t regs[32];      //Registers, if you want to modify T0 you can use regs[T0]
Ifid IFID;
Idex IDEX;
Exmem EXMEM;
Memwb MEMWB;
uint32_t PC;
#endif
