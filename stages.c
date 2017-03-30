#include "stages.h"
#include "cpuSim.h"

void FetchStage(){
  setCurIns();
}

void DecodeStage(){
  setControls(curIns.opcode); //returns all of the control line values set
  //Read 2 registers to pass to ALU or read one register and use signExt unit for immediate value
  int arg1, arg2;
  arg1 = regs[curIns.rs]; //r1 is always element in register file with index curIns.rs
  arg2 = mux(signExt(curIns.imm), regs[curIns.rt], controlUnit.ALUsrc); //ALU unit input mux
  ALU(arg1, arg2);
}
void ExecuteStage(){

}
void MemoryStage(){

}
void WritebackStage(){

}

//read memory based on PC, based off of opcode divide bits into different parts
int setCurIns(){
  uint32_t ci = memory[PC];  //current instruction
  uint8_t opcode = (ci & 0xfc000000)>>26; //most significant 6 bits
  if(opcode==0x0){
    //printf("R instruction\n");
    curIns.rs = (ci & 0x03E00000)>>21;
    curIns.rt = (ci & 0x001F0000)>>16;
    curIns.rd = (ci & 0x0000F800)>>11;
    curIns.shamt = (ci & 0x000007C0)>>6;
    curIns.funct = (ci & 0x0000003F);
  } else if (opcode==0x2 || opcode==0x3){
    //printf("J instruc\n");
    curIns.addr = (ci & 0x03FFFFFF);
  } else {
    //printf("I instruc\n");
    curIns.rs = (ci & 0x03E00000)>>21;
    curIns.rt = (ci & 0x001F0000)>>16;
    curIns.imm = (ci & 0x0000ffff);
  }
  #if DEBUG==1
  printf("\n***setCurIns***\n");
  if(opcode==0x0) printf("R function\nRS: %d\nRT: %d\nRD: %d\nShamt: %d\nFunct: %d\n", curIns.rs, curIns.rt, curIns.rd, curIns.shamt, curIns.funct);
  else if(opcode==0x2||opcode==0x3) printf("J instruction\nAddress w/o shift: %04X", curIns.addr);
  else printf("I instruction\nOpcode: %d\nRS: %d\nRT: %d\nImm: %d\n", opcode, curIns.rs, curIns.rt, curIns.imm);
  #endif
  return 0;
}
