#include "cpuSim.h"
#include "registers.h"
#define Nop() {_asm nop _endasm}

//#define branchsubunit
//Initialize structs
void init(){
  memset(&controlUnit, 0, sizeof(controlUnit));
  memset(&IFID, 0, sizeof(IFID));
  memset(&IDEX, 0, sizeof(IDEX));
  memset(&EXMEM, 0, sizeof(EXMEM));
  memset(&MEMWB, 0, sizeof(MEMWB));
  memset(memory, 0, MEMSIZE);
  memset(&regfile, 0, sizeof(regfile));
  WasBranch = 0;
  ShouldExec = 1;
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
int setCurIns(uint32_t ci){
  uint8_t opcode = (ci & 0xfc000000)>>26; //most significant 6 bits
  IFID.ins.opcode = opcode;
  if(opcode==0x0){
    //printf("R instruction\n");
    IFID.ins.rs = (ci & 0x03E00000)>>21;
    IFID.ins.rt = (ci & 0x001F0000)>>16;
    IFID.ins.rd = (ci & 0x0000F800)>>11;
    IFID.ins.shamt = (ci & 0x000007C0)>>6;
    IFID.ins.funct = (ci & 0x0000003F);
    IFID.ins.imm = 0; //rtypes don't have imm
  } else if (opcode==0x2 || opcode==0x3){
    //printf("J instruc\n");
    IFID.ins.addr = (ci & 0x03FFFFFF);
    IFID.ins.rs = 0;
    IFID.ins.rt = 0;
    IFID.ins.rd = 0;
    IFID.ins.shamt = 0;
    IFID.ins.funct = 0;
    IFID.ins.imm = 0; //rtypes don't have imm
  } else {
    //printf("I instruc\n");
    IFID.ins.rs = (ci & 0x03E00000)>>21;
    IFID.ins.rt = (ci & 0x001F0000)>>16;
    IFID.ins.imm = (ci & 0x0000ffff);
    IFID.ins.rd = 0; //itypes don't have rd
    IFID.ins.shamt = 0; //itypes don't have shamt
    IFID.ins.funct = 0; //itypes don't have funct
  }

//  printf("\nopcode: %d\nrd: %d\nrs: %d\nrt: %d\nshamt: %d\nfunct: %d\nimmed: %d\n",IFID.ins.opcode,IFID.ins.rd,IFID.ins.rs,IFID.ins.rt,IFID.ins.shamt,IFID.ins.funct,IFID.ins.imm);
  //printf("\nopcode: %d, rd: %d, rs: %d, rt: %d, shamt: %d, funct: %d, immed: %d\n",IFID.ins.opcode,IFID.ins.rd,IFID.ins.rs,IFID.ins.rt,IFID.ins.shamt,IFID.ins.funct,IFID.ins.imm);

  return 0;
}

/*
*  i1 and i2 are register numbers, not values in registers.
GOOD POINT. -vanika
*  jump and branch info: http://stackoverflow.com/questions/6950230/how-to-calculate-jump-target-address-and-branch-target-address
*/

void BranchUnit (uint32_t zerosig, uint32_t signeximm) {

//subtract the two inputs -> done outside of this function
//uint32_t output = controlUnit.Branch & sub
    printf("zerosignal: %d, controlUnit.branch: %d\n",zerosig,controlUnit.Branch);
    printf("Branch!!!!!\n");
    printf("Old PC: %d\n",PC);
    uint32_t input1 = PC + signeximm; //result of the alu
    uint32_t input0 = PC; //these are the incremented versions of PC, different than the actual PC we are on.
    printf("signeximm: %d, input0: %d, input1: %d\n",signeximm,input0,input1);
    uint32_t s = controlUnit.Branch & zerosig;
    //if v1 == v2, 1 & 1 = 1, we choose the branched address
    //if v1 != v2, 1 & 0 = 0, we chose just PC + 4.
    PC = mux(input0, input1, s); //ALU unit input mux
    //PC = IDEX.PC;
    printf("New PC: %d\n",PC);

}

int32_t ALUfunct(int32_t v1, int32_t v2, uint8_t * err_p, uint8_t resReg) {
  //for R-type: i1 = rs, i2 = rt, resReg = rd
  //for I-type: i1 = rs, i2 = imm, resReg = rt

  EXMEM.nopFlag = 0; //no-op is level sensitive to high
  //EXMEM.skipMem = 1; //set to 1 as generally we will skipmem unless lw or sw

  uint8_t err = * err_p;
  //int32_t res = 0;
  //uint8_t oper = 0x0;
  //uint32_t sub;
  //cases in book covered by ALUop
  switch(controlUnit.ALUop){

    case 0b00: //load/store
      //printf("SW: v1:%d, v2:%d,alu.ALUres:%d\n",v1,v2,alu.ALUres);
      alu.ALUres = v1 + v2; //res = reg[rs] + signext(offset)
      break;



    case 0b01: //branch

      #ifdef branchsubunit
      alu.zero = (v1-v2 == 0) ? 1 : 0; //if v1 = v2 ret 1, else ret 0
      BranchUnit(alu.zero,v2);
      #endif // branchsubunit


      break;

    case 0b10: //R-type - analyze funct field

      switch(IDEX.ins.funct) {
        case 0b100000: //add (0x20)
            alu.ALUres = v1 + v2;
            break;

        case 0b100010: //subtract (0x22)
            alu.ALUres = v1 - v2;
            break;

        case 0b100100: //bit-wise AND (0x24)
            alu.ALUres = v1 & v2;
            break;

        case 0b100101: //bit-wise OR (0x25)
            alu.ALUres = v1 | v2;
            break;

        case 0b101010: //set on less than - slt (0x2a)
            alu.ALUres = v1 < v2 ? 1 : 0;
            break;

        //additional cases not covered in textbook:
        case 0x21: //addu
            //TODO: will this be stored in res correctly?
            alu.ALUres = (uint32_t)((uint32_t)v1 + (uint32_t)v2);
            break;

        case 0x07: //nor
            printf("INFO: Hit \"nor\" instruction with ALUop value as expected for an R instruction, see confusion in comment, delete print if seen\n");
            alu.ALUres = ~(v1 | v2);
            break;
        case 0x2b: //sltu
            alu.ALUres = v1 < v2 ? 1 : 0;
            break;
        case 0x00: //sll
            alu.ALUres = v1 << IDEX.ins.shamt;
            if(IDEX.ins.shamt == 0) EXMEM.nopFlag = 1;
            break;
        case 0x02: //srl
            alu.ALUres = v1 >> IDEX.ins.shamt;
            break;
        case 0x23: //subu TODO: am I casting correctly?
            //VANIKA - fix later.
            alu.ALUres = (uint32_t)((uint32_t)v1 - (uint32_t)v2);
            break;
        case 0x26: //xor. 38. 10 0110
            alu.ALUres = v1 ^ v2;
            break;
        case 0b001011: //movn
            //is v2 not equal to 0? if it is, store v2'd value in rd
            //if v2 = 0, store rt's value back into rd
            alu.ALUres = (v2 != 0) ? v2 : v1;
            break;
        case 0b001010: //movz
            alu.ALUres = (v2 == 0) ? v2 : v1; // if rt == 0 store rd's value in rs.
            //else just store the current rd value back in rd.
            break;
        case 0b001000:
            printf("jump!\n");
            break;
      }
      break; //ending r-type instruction



    /* ***************************************************************
    I-type
    ***************************************************************** */

   default:
       //for everything else beyond r-format instructions, and lw- and sw- specifically:
       //we will analyze the opcode of the instruction. these should all be either i or j format.
       switch(IDEX.ins.opcode) {
        case 0x8: //addi
            alu.ALUres = v1 + v2; //rt = rs + imm (what does "with overflow" mean?)
            break;
        case 0x9: //addiu
            //check for overflow conditions first (do after you figure out subu addu situation)
            alu.ALUres = v1 + v2; //rt = rs + imm (without overflow)
            break;
        case 0xc: //andi
            alu.ALUres = v1 & v2; //rt = rs & imm
            break;
        case 0x4: //beq
            //already done
            printf("Error with beq instruction."); //-> since it should have already been done before
            //if rs = rt pc = offset << 2.
            break;
        case 0x5: //bne
            //same logic as above, except we just check if they aren't equal
            #ifdef branchsubunit
            alu.zero = v1-v2;
            alu.zero = (alu.zero != 0) ? 1 : 0; //if v1 = v2 ret 1, else ret 0
            BranchUnit(alu.zero,v2);
            #endif // branchsubunit

            break;
        case 0x24: //lbu
            alu.ALUres = v1 + v2; //res is the memory location for mem to reference. we don't sign extend here since it's talking about a byte
            //EXMEM.skipMem = 0;
            break;
        case 0x25: //lhu
            alu.ALUres = v1 + v2;
            //EXMEM.skipMem = 0;
            break;
        case 0xf: //lui
            alu.ALUres = v1 + v2;
            //EXMEM.skipMem = 0;
            break;
        //lw
        case 0xd: //ori
            alu.ALUres = v1 | v2;
            break;
        case 0xa: //slti
            alu.ALUres = v1 < v2 ? 1:0; //results in stored value in res register
            break;
        case 0xb: //sltiu
            alu.ALUres = v1 < v2 ? 1:0;
            break;
        case 28: //sb
            alu.ALUres = v1 + v2;
            //EXMEM.skipMem = 0;
            EXMEM.write_data = v2;
            break;
        case 29: //sh
            alu.ALUres = v1 + v2;
            //EXMEM.skipMem = 0;
            EXMEM.write_data = v2;
            break;
        //sw
        case 0b001110: //xori, 15
            alu.ALUres = v1 ^ v2;
            break;
        case 0b000111: //bgtz, 7
            #ifdef branchsubunit
            alu.zero = (v1 > 0) ? 1: 0;
            BranchUnit(alu.zero,v2);
            #endif // branchsubunit
            break;
        case 0b000001: //bltz, 1
            #ifdef branchsubunit
            alu.zero = (v1 < 0) ? 1: 0;
            BranchUnit(alu.zero,v2);
            #endif // branchsubunit
            break;
        case 0b000110: //blez, 6
            #ifdef branchsubunit
            alu.zero = (v1 <= 0) ? 1: 0;
            //printf("sub: %d\n",sub);
            BranchUnit(alu.zero,v2);
            #endif // branchsubunit
            break;
        case 0b100000: //lb, 32
            alu.ALUres = v1 + v2;
            break;

     // error code
        default:
          printf("Instruction not found.");
          err = 1;
          return;
      }
  } //outer switch statement
  if (err) {
    printf("Error with ALU control unit. Please check Opcode and funct field detection.\n");
    return -1;
  }

  *err_p = err;

  //Set pipeline register values
  /*int32_t ALUres;
  uint8_t zero;
  int32_t write_data;
  uint32_t addr;
  uint32_t PC;
  uint8_t skipMem;
  uint8_t nopFlag;
  */
  printf("ALU result: %d\n",alu.ALUres);
  return err;
}
int32_t signExt(int16_t offsetField) {
    uint32_t ans;
    uint8_t sign = (offsetField >> 15) & 0x1;
    uint32_t mask = sign?0xffff0000:0x00000000;
    ans = (int32_t)(offsetField|mask);
    return ans;

}

int dataMemoryUnit(int32_t addr, int32_t writeData){
  return 0;
}

int32_t mux(int32_t zero, int32_t one, uint8_t ctrl){ //zero is rt, one is imm
  return ctrl?one:zero; //if ctrl = 1 choose one and if ctril = 0 then choose zero
}

//Load instruc from i-cache
void c_loadi(){

}

//Store instruc in i-cache
void c_storei(){

}

//Store data in d-cache
void c_stored(){

}

//Load data from d-cache
void c_loadd(){

}

void store(){
  dataMem[EXMEM.ALUres>>2] = EXMEM.write_data;
}

int32_t load() {
  return dataMem[EXMEM.ALUres>>2];
}
