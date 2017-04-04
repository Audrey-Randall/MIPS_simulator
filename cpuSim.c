#include "cpuSim.h"

//Initialize structs
void init(){
  memset(&controlUnit, 0, sizeof(controlUnit));
  memset(&IFID, 0, sizeof(IFID));
  memset(&IDEX, 0, sizeof(IDEX));
  memset(&EXMEM, 0, sizeof(EXMEM));
  memset(&MEMWB, 0, sizeof(MEMWB));
  memset(&curIns, 0, sizeof(curIns));
  PC = 0;

  curIns.opcode = 0;
  curIns.rs = 0;
  curIns.rt = 0;
  curIns.rd = 0;
  curIns.shamt = 0;
  curIns.funct = 0;
  curIns.imm = 0;
  curIns.signextimm = 0;
  curIns.addr = 0;
}


int32_t ALU(uint8_t input1, uint8_t input2, uint8_t err, uint8_t result) {

    //for R-type: input1 = rs, input2 = rt, result = rd
    //for I-type: input1 = rs, input2 = imm, result = rt
  //return x ALUop y
  uint8_t oper = 0x0;
        printf("controlUnit.ALUop: %d\n",controlUnit.ALUop);
  switch(controlUnit.ALUop){

    case 0b00: //this means that it's a load/store instruction (I-type)
      oper = 0x2; //(0010) - add
      break;
    case 0b01: //this means that it's a branch statement (I-type)
      oper = 0x6; // (0110) - subtract (check this. why would we subtract on a branch statement?)
      break;
    case 0b10: //this means that it's an R-type instruction
        printf("curIns.funct: %d\n",curIns.funct);
      //analyze funct field
      switch(curIns.funct) {
        case 0b100000: //add (0x20)
            oper = 0x2; //(0010) - add
            break;
        case 0b100010: //subtract (0x22)
            oper = 0x6; //(0110) - subtract
            break;
        case 0b100100: //bit-wise AND (and with 0 opcode) (0x24)
            oper = 0x0; //(0000) - bitwise AND
            break;
        case 0b100101: //bit-wise OR (or with 0 opcode) (0x25)
            oper = 0x1; //(0001) - bitwise OR
            break;
        case 0b101010: //set on less than - slt (0x2a)
            oper = 0x111; //(0111) - set on less than
            break;

        //additional cases not covered in textbook:
        case 0x21: //addu
            oper = 0x2;
                        printf("not done yet");
            break;


        case 0x08: //jr (this is an R-type instruction)
            PC = input1; //pc = rs
                        printf("not done yet");
            break;


        case 0x07: //nor //this is technically R-type, however in this case the controlUnit.ALUop will be different...do we need this?
            input1 = ~input1;
            input2 = ~input2;
            oper = 0x0; //(AND) - using demorgans, ~(arg1 + arg2) = ~arg1 * ~arg2
                        printf("not done yet");
            break;

        case 0x2b: //sltu
            if (input1 < input2) { //if rs < rt
                input1 = 1;
                input2 = 0;
                //store rd = 1+0 = 1 in rd
            }
            else {
                input1 = 0;
                input2 = 0;
                //store rd = 0 + 0 = 0 in rd
            }
            oper = 0x2;
                                    printf("not done yet");
            break;

/*
        case 0x00: //sll -> is this allowed?
            //sll is the same thing as taking rt, and multiplying it by shamt. i.e. 4 << 2 = 0b100 << 2 = 0b10000 = 16, = 4 * 2^shamt
            //since we don't have an ALU functionality to multiply things, we have to add 4 + 4 + 4 + 4 aka do 4*4 that way.

            printf("not done yet");
            break; */

        case 0x02: //srl
            printf("not done yet");
            break;

        case 0x23: //subu
            oper = 0x6;
            printf("not done yet");
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

  switch(oper) {

    //add
    case 0010:
        result = input1 + input2;
        PC+=4;
        break;
    //subtract
    case 0110:
        result = input1 - input2;
        PC+=4;
        break;
    //AND
    case 0000:
        result = input1 & input2;
        PC+=4;
        break;
    //OR
    case 0001:
        result = input1 | input2;
        PC+=4;
        break;
    //set on less than
    case 0111:
        input1 < input2 ? result:result;
        PC+=4;
        break;
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
    ???? */

}

int dataMemoryUnit(int32_t addr, int32_t writeData){
  return 0;
}

int32_t mux(int32_t zero, int32_t one, uint8_t ctrl){ //zero is rt, one is imm
  return ctrl?one:zero; //if ctrl = 1 choose one and if ctril = 0 then choose zero
}

