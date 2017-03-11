#include "cpuSim.h"

//Initialize structs
void init(){
  memset(&controlUnit, 0, sizeof(controlUnit));
  memset(&IFID, 0, sizeof(IFID));
  memset(&IDEX, 0, sizeof(IDEX));
  memset(&EXMEM, 0, sizeof(EXMEM));
  memset(&MEMWB, 0, sizeof(MEMWB));
}

int parse(char* file) {
  return 0;
}

int32_t ALU(int x, int y) {
  //return x ALUop y
  uint8_t oper = 0x0;
  switch(controlUnit.ALUop){
    case 0b00:
      oper = 0x2;
      break;
    case 0b01:
      oper = 0x6;
      break;
    case 0b10:
      //switch (funct field of instruc)
      break;
    case 0b11:
      printf("ERROR: ALUop was 0x11, Audrey didn't think this was possible\n");
  }
  return 0;
}

int32_t signExt(int32_t offsetField) {
  uint8_t sign = offsetField >> 15;
  uint32_t mask = sign?0xffff0000:0x00000000;
  return (int32_t)(offsetField|mask);
}

int dataMemoryUnit(int32_t addr, int32_t writeData){
  return 0;
}

int32_t mux(int32_t zero, int32_t one, uint8_t ctrl){
  return ctrl?one:zero;
}


