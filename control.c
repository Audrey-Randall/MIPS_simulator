
#include "control.h"
#include "cpuSim.h"

//Figure 4.22 tells how to implement this
int setControls(uint8_t opcode){
  switch(opcode){
    case OPCODE_R:
      controlUnit.RegDst = 1;
      controlUnit.ALUsrc = 0;
      controlUnit.MemToReg = 0;
      controlUnit.RegWrite = 1;
      controlUnit.MemRead = 0;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 0;
      controlUnit.ALUop = 0b10;
      break;
    case OPCODE_LW:
      controlUnit.RegDst = 0;
      controlUnit.ALUsrc = 1;
      controlUnit.MemToReg = 1;
      controlUnit.RegWrite = 1;
      controlUnit.MemRead = 1;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 0;
      controlUnit.ALUop = 0b00;
      break;
    case OPCODE_SW:
      controlUnit.RegDst = 0;  //X in diagram in book
      controlUnit.ALUsrc = 1;
      controlUnit.MemToReg = 0;  //X in book
      controlUnit.RegWrite = 0;
      controlUnit.MemRead = 0;
      controlUnit.MemWrite = 1;
      controlUnit.Branch = 0;
      controlUnit.ALUop = 0b00;
      break;
    case OPCODE_BEQ:
      controlUnit.RegDst = 0;  //X in book
      controlUnit.ALUsrc = 0;
      controlUnit.MemToReg = 0;  //X in book
      controlUnit.RegWrite = 0;
      controlUnit.MemRead = 0;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 1;
      controlUnit.ALUop = 0b01;
      break;
    default:
      return -1;
  }
  return 0;
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

  printf("opcode: %d\n",curIns.opcode);
  printf("rs: %d\n",curIns.rs);
  printf("rt: %d\n",curIns.rt);
  printf("rd: %d\n",curIns.rd);
  printf("shamt: %d\n",curIns.shamt);
  printf("funct: %d\n",curIns.funct);
  printf("imm: %d\n",curIns.imm);
  printf("addr: %d\n",curIns.addr);
  return 0;
}
