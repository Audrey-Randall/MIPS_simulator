#include "cpuSim.h"

//Initialize structs
void init(){
  memset(&controlUnit, 0, sizeof(controlUnit));
  memset(&IFID, 0, sizeof(IFID));
  memset(&IDEX, 0, sizeof(IDEX));
  memset(&EXMEM, 0, sizeof(EXMEM));
  memset(&MEMWB, 0, sizeof(MEMWB));
  memset(&curIns, 0, sizeof(curIns));
  memset(memory, 0, MEMSIZE);
  PC = 0;
}

int parseInput(char* inFile){
  FILE* fp = fopen(inFile, "r");
  int line = 0;
  if(!fp) {
    perror("ERROR: cannot open input file");
    exit(1);
  }
  char buf[512];
  memset(buf, 0, sizeof(buf));

  //Read in line by line, buf will contain each line including the newline character
  while(fgets(buf, sizeof(buf), fp)) {
    char instr[12];
    char trash[500];
    if(sscanf(buf, "%s %s", instr, trash) < 2) {
      //printf("Line %d in input file became %s and %s\n", line, instr, trash);
    } else {
      instr[10] = 0;
      memory[line] = (uint32_t)strtoul(instr, NULL, 16);
    }
    line++;
    memset(instr, 0, sizeof(instr));
  }
  fclose(fp);
  return line;
}

//read memory based on PC, based off of opcode divide bits into different parts
int setCurIns(){
  uint32_t ci = memory[PC];  //current instruction
  uint8_t opcode = (ci & 0xfc000000)>>26; //most significant 6 bits
  curIns.opcode = opcode;
  printf("OPCODE:");
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
  #ifdef DEBUG
  printf("\n***setCurIns***\n");
  if(opcode==0x0) printf("R function\nRS: %d\nRT: %d\nRD: %d\nShamt: %d\nFunct: %d\n", curIns.rs, curIns.rt, curIns.rd, curIns.shamt, curIns.funct);
  else if(opcode==0x2||opcode==0x3) printf("J instruction\nAddress w/o shift: %04X", curIns.addr);
  else printf("I instruction\nOpcode: %d\nRS: %d\nRT: %d\nImm: %d\n", opcode, curIns.rs, curIns.rt, curIns.imm);
  #endif

  return 0;
}

/*
*  i1 and i2 are register numbers, not values in registers.
*  jump and branch info: http://stackoverflow.com/questions/6950230/how-to-calculate-jump-target-address-and-branch-target-address
*/
int32_t ALU(uint8_t i1, uint8_t i2, int32_t imm, uint8_t err, uint8_t resReg) {
  //for R-type: i1 = rs, i2 = rt, resReg = rd
  //for I-type: i1 = rs, i2 = imm, resReg = rt
  int32_t res = 0;
  uint8_t oper = 0x0;
  printf("controlUnit.ALUop: %d\n",controlUnit.ALUop);

  switch(controlUnit.ALUop){
    case 0b00: //this means that it's a load/store instruction (I-type)
      oper = 0x2; //(0010) - add
      res = regs[i1] + imm;
      break;
    case 0b01: //this means that it's a branch statement (I-type)
      oper = 0x6; // (0110) - subtract
      res = PC + 4 + (imm<<2);
      break;
    case 0b10: //this means that it's an R-type instruction
      printf("curIns.funct: %d\n",curIns.funct);
      int32_t v1 = regs[i1];
      int32_t v2 = regs[i2];
      //analyze funct field
      switch(curIns.funct) {
        case 0b100000: //add (0x20)
            oper = 0x2; //(0010) - add
            res = v1 + v2;
            break;
        case 0b100010: //subtract (0x22)
            oper = 0x6; //(0110) - subtract
            res = v1 - v2;
            break;
        case 0b100100: //bit-wise AND (and with 0 opcode) (0x24)
            oper = 0x0; //(0000) - bitwise AND
            res = v1 & v2;
            break;
        case 0b100101: //bit-wise OR (or with 0 opcode) (0x25)
            oper = 0x1; //(0001) - bitwise OR
            res = v1 | v2;
            break;
        case 0b101010: //set on less than - slt (0x2a)
            oper = 0b111; //(0111) - set on less than
            res = v1 < v2 ? 1 : 0;
            break;

        //additional cases not covered in textbook:
        case 0x21: //addu
            oper = 0x2;
            //TODO: will this be stored in res correctly?
            res = (uint32_t)((uint32_t)v1 + (uint32_t)v2);
            break;
        case 0x08: //jr (this is an R-type instruction)
            PC = v1;
            break;
        //Shift right arithmetic by variable bits: srav. TODO: figure out which it is.
        case 0x07: //nor. This is technically R-type, however in this case the controlUnit.ALUop will be different...do we need this?
            printf("INFO: Not implemented\n");
            v1 = ~v1;
            v2 = ~v2;
            oper = 0x0; //(AND) - using demorgans, ~(arg1 + arg2) = ~arg1 * ~arg2
            break;
        case 0x2b: //sltu
            res = v1 < v2 ? 1 : 0;
            oper = 0x2;
            break;
        case 0x00: //sll -> is this allowed?
            res = v1 << curIns.shamt;
            break;
        case 0x02: //srl
            res = v1 >> curIns.shamt;
            break;
        case 0x23: //subu TODO: am I casting correctly?
            oper = 0x6;
            res = (uint32_t)((uint32_t)v1 - (uint32_t)v2);
            break;

        default:
            printf("none of the above.\n");
            err = 1;
      }
      break;

   default:
       //assuming that I-format instructions are based off of opcode and not ALUop
       //as implied on:https://www.d.umn.edu/~gshute/mips/single-cycle-summary.pdf
       //otherwise, I'm not totally sure what the ALUsrc signals should be for I-type instructions in general.
       //might be in the appendix??

       switch(curIns.opcode) {
        case 0x8: //addi
            break;
        case 0x9: //addiu
            break;
        case 0xc: //andi
            break;
        case 0x4: //beq
            break;
        case 0x5: //bne
            break;
        case 0x24: //lbu
            break;
        case 0x25: //lhu
            break;
        case 0x30: //ll
            break;
        case 0xf: //lui
            break;
        //lw
        case 0xd: //ori
            break;

        ////////etc//////////

       }


        err = 1;
        printf("J-type?\n");
        return;
  }
  if (err == 1) {
    printf("Error with ALU control unit. Please check Opcode and funct field detection.\n");
    return -1;
  }
  return result;
}
int32_t signExt(int16_t offsetField) {
    uint32_t ans;
    uint8_t sign = (offsetField >> 15) & 0x1;
    uint32_t mask = sign?0xffff0000:0x00000000;
    ans = (int32_t)(offsetField|mask);
    return ans;

    /*could we not use:
    int32_t ans = offsetField;
    return ans;
    ???
    No- the sign of negative numbers won't be extended. C's casting doesn't do that for you. */

}

int dataMemoryUnit(int32_t addr, int32_t writeData){
  return 0;
}

int32_t mux(int32_t zero, int32_t one, uint8_t ctrl){ //zero is rt, one is imm
  return ctrl?one:zero; //if ctrl = 1 choose one and if ctril = 0 then choose zero
}
