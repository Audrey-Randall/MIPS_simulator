
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
      printf("R-format\n");
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
      printf("LW-format\n");
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
      printf("SW-format\n");
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
      printf("BEQ-format\n");
      break;
    default:printf("none of the above.\n");
      return -1;
  }
  return 0;
}

//read memory based on PC, based off of opcode divide bits into different parts
int setCurIns(){
  uint32_t ci = memory[PC];  //current instruction
  uint8_t opcode = (ci & 0xfc000000)>>26; //most significant 6 bits
  curIns.opcode = opcode;
  printf("OPODE:");
  if(opcode==0x0){
    //printf("R instruction\n");
    curIns.rs = (ci & 0x03E00000)>>21;
    curIns.rt = (ci & 0x001F0000)>>16;
    curIns.rd = (ci & 0x0000F800)>>11;
    curIns.shamt = (ci & 0x000007C0)>>6;
    curIns.funct = (ci & 0x0000003F);
    curIns.imm = 0; //rtypes don't have imm
    InstructionType = Rtype;
  } else if (opcode==0x2 || opcode==0x3){
    //printf("J instruc\n");

    curIns.addr = (ci & 0x03FFFFFF);
    InstructionType = Jtype;
  } else {
    //printf("I instruc\n");
    curIns.rs = (ci & 0x03E00000)>>21;
    curIns.rt = (ci & 0x001F0000)>>16;
    curIns.imm = (ci & 0x0000ffff);
    curIns.rd = 0; //itypes don't have rd
    curIns.shamt = 0; //itypes don't have shamt
    curIns.funct = 0; //itypes don't have funct
    InstructionType = Itype;
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

//#define printinstructionmemory

int textFileConversion(FILE *fp) {
    int i;
    char input[1000]; //to store each char into an array (to make it a string)
    char * hexarray[8]; //array of strings
    int c;
    int status1;
    uint32_t number;
    fp = fopen("Simulation_example.txt","r");
    if (fp == NULL) {
        printf("cannot open file.\n");
        return -1;
    }

    //store each character into a string, print each line
    else {
        do
        {
            status1 = fgets(input, sizeof(input), fp); //status returns the array of chars where the string read is stored. it returns NULL if error occurs
            //printf("STATUS1: %d",status1);
            number = strtol(input, NULL, 0); //converts hex string number to int
            //printf(" NUMBER: %ld",number);
            hexarray[index] = input;
            memory[index] = number;
            //printf(" MEMORY INDEX: %ld\n",memory[index]);
            index++; //counts how many instructions are in text file.
        }
        while (status1 || (c = getc(fp)) != ',' && c != EOF);
    }
    fclose(fp);

    #ifdef printinstructionmemory
    //print integer array
    for(i=0; i<index-1; i++) {
        printf("element %d in dec: %d\n",i,memory[i]);
    }

    #endif // printinstructionmemory

    return 0;
}
