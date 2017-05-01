#include "stages.h"
#include "cpuSim.h"
#include "control.h"

void printCurrentInstruction(){
  if(IFID.ins.opcode==0x0){
    printf("R instruction. Rs = %d, Rt = %d, Rd = %d, funct = 0x%x.\n", IFID.ins.rs, IFID.ins.rt, IFID.ins.rd, IFID.ins.funct);
  } else if(IFID.ins.opcode==0x2 || IFID.ins.opcode==0x3) {
    printf("J instruction opcode 0x%08X. Addr = %d\n", IFID.ins.opcode, IFID.ins.addr);
  } else {
    printf("I instruction opcode 0x%08X. Rs = %d, Rt = %d, Imm = %d\n", IFID.ins.opcode, IFID.ins.rs, IFID.ins.rt, IFID.ins.imm);
  }
}

void FetchStage(){
    //branch control logic
    if (BranchAddr && amTagalong) {
      amTagalong = 0;
    } else if (BranchAddr && !amTagalong) {
	//I'm the intruction after the tagalong: set the PC to BranchAddr
        PC = BranchAddr;
	BranchAddr = 0;
    }
    //1. Instruction is read from memory using the address in the PC
    ci = memory[PC>>2];  //current instruction
    //2. Instruction itself is stored into IFID register.
    IFID.ins.instruct = ci;
    //3. PC incremented, stored to pipeline, for need in things like branch instruction
    printf("\n\n\nmemory[%d]: 0x%08X-------------------------------------------------\n",PC>>2, memory[PC>>2]);
    PC+=4;
    IFID.PC = PC;
    printf("FETCH: \n\tInstruction = 0x%x\n\tPC = %d\n", ci, IFID.PC-4);
}

void printControls(){
  printf("Controls:  ALUop = %d, ALUsrc= %d, RegWrite = %d, MemWrite = %d, MemToReg = %d, MemRead = %d,PCsrc = %d, RegDst = %d, Branch = %d, Jump = %d\n",
  controlUnit.ALUop, controlUnit.ALUsrc, controlUnit.RegWrite,controlUnit.MemWrite, controlUnit.MemToReg, controlUnit.MemRead, controlUnit.PCsrc, controlUnit.RegDst, controlUnit.Branch, controlUnit.Jump);
}

int isLoadOrStore(uint16_t oc){
  //Loads
  if(oc == 0x23 || oc == 0x24 || oc == 0x25 || oc == 0xf) return Loads;
  //Stores
  if(oc == 0x28 || oc == 0x29 || oc == 0x2b) return stores;
  //Everything else
  return 0;
}

void DecodeStage() {
    printf("DECODE: \n\t");
    //1. Instruction decode. Register numbers, imm fields, etc. supplied.
    setCurIns(ci);
    printCurrentInstruction();

    //branch variables
    uint8_t zero=0;
    int32_t v1 = regfile.regs[IFID.ins.rs];
    int32_t v2 = regfile.regs[IFID.ins.rt];
    int32_t signext = signExt(IFID.ins.imm);
    printf("\tv1 = %d and v2 = %d\n", v1, v2);

    //TODO: check if jump instruc should make rest of stages be nops


    //2. Based off of this decoding, set control lines
    setControls(IFID.ins.opcode);
    printf("\t");
    printControls();

    //3. Register numbers fed into register file.
    regfile.readreg1 = IFID.ins.rs;
    regfile.readreg2 = mux(IFID.ins.rt, IFID.ins.rd, controlUnit.RegDst);
    printf("decode stage: readreg1 = %d, readreg2 = %d\n",regfile.readreg1, regfile.readreg2);

    //4. Register values read and stored into pipeline register
    regfile.regdata1 = regfile.regs[regfile.readreg1];
    regfile.regdata2 = regfile.regs[regfile.readreg2];
    IDEX.regdata1 = regfile.regdata1;
    IDEX.regdata2 = regfile.regdata2;

    //5. Store sign extended imm field into pipeline reg
    IDEX.ins.signextimm = signExt(IFID.ins.imm);

    //6. Pass everything else onward
    IDEX.ins.instruct = IFID.ins.instruct;
    IDEX.ins.opcode = IFID.ins.opcode;
    IDEX.ins.rs = IFID.ins.rs;
    IDEX.ins.rt = IFID.ins.rt;
    IDEX.ins.rd = IFID.ins.rd;
    IDEX.ins.shamt = IFID.ins.shamt;
    IDEX.ins.funct = IFID.ins.funct;
    IDEX.ins.imm = IFID.ins.imm;
    IDEX.ins.addr = IFID.ins.addr;
    IDEX.PC = IFID.PC; //again, this is the incremented one. not the current inst. If a jump changes it it gets reset later
    IDEX.nopFlag = 0;

    //7. to use later for stores
    if (isLoadOrStore(IFID.ins.opcode)) {
        IDEX.write_to_mem_reg = IFID.ins.rt; //the register number that we will write in a storeword instruction
        IDEX.write_to_mem_val = regfile.regs[IDEX.write_to_mem_reg]; //store value of this reg
    }

    //9. Jump / Branches
    if (controlUnit.Jump) {
	amTagalong = 1;
        if (IDEX.ins.opcode == 0x2) {
            printf("JMP: IFID.ins.addr before editing: %08X\n",IFID.ins.addr);
            uint32_t ms4 = (uint32_t)(IFID.ins.addr & 0xf0000000); //select MS four bits
            uint32_t abbrevAddr = (IFID.ins.addr & 0x03ffffff) << 2;
            uint32_t finaladdress = (ms4 | abbrevAddr);
            printf("ms4: %08X, abbrevAddr: %08X, finaladd: %08X\n", ms4, abbrevAddr, finaladdress);
            BranchAddr = finaladdress; //jump to this address
	    amTagalong = 1;
	    IDEX.nopFlag = 1;
            printf("ENDING PC: %d and addr to jump to next time: 0x%x\n", PC, BranchAddr);

        }
        else if (IDEX.ins.opcode = 0x3) {
            printf("JAL: IFID.ins.addr before editing: %08X\n",IFID.ins.addr);
            uint32_t temp1 = (uint32_t)(IFID.ins.addr & 0xf0000000);
            uint32_t temp2 = (IFID.ins.addr & 0x03ffffff) << 2;
            uint32_t finaladdress = (temp1 | temp2);
            printf("temp1: %08X, temp2: %08X, finaladd: %08X\n",temp1,temp2, finaladdress);
            regfile.regs[RA] = PC;//should be instruction after current one but we increment PC in fetch
            printf("Stored %d (PC) to reg %d\n",PC,RA);
            //note that we need to add a branch delay slot after this.
            BranchAddr = finaladdress; //jump to this address
	    amTagalong = 1;
	    IDEX.nopFlag = 1;
            printf("ENDING PC: %d and addr to jump to next time: 0x%x\n", PC, BranchAddr);
        }
        return;
    }

    //branch logic
    if (controlUnit.Branch) {
        printf("BRAAAAAAAAAANCH\n");
        IDEX.nopFlag = 1;
        printf("IDEX.ins.opcode: %d\n",IDEX.ins.opcode);

        if (IDEX.ins.opcode == OPCODE_BEQ) {
            zero = (v1-v2 == 0) ? 1 : 0; //if v1 = v2 ret 1, else ret 0
            BranchUnit(zero,signext);
        }
        else if (IDEX.ins.opcode == OPCODE_BGTZ) {
            printf("BGTZ:v1: %d\n",v1);
            zero = (v1 > 0) ? 1: 0;
            BranchUnit(zero,signext);
        }
        else if (IDEX.ins.opcode == OPCODE_BLEZ) {
            printf("BLEZ: v1: %d\n",v1);
            zero = (v1 <= 0) ? 1: 0;
            BranchUnit(zero,signext);
        }
        else if (IDEX.ins.opcode == OPCODE_BNE) {
            zero = (v1-v2 != 0) ? 1 : 0; //if v1 = v2 ret 1, else ret 0
            BranchUnit(zero, signext);
        }
        else if (IDEX.ins.opcode == OPCODE_BLTZ) {
            zero = (v1 < 0) ? 1: 0;
            BranchUnit(zero,signext);
        }
        else printf("Error with branching in decode stage. Please check logic.\n");
    }
    IDEX.PC = PC;
}

void ExecuteStage(){
    int i;
    uint8_t err = 0;

    //check for nop
    if(IDEX.nopFlag) return;
    if(controlUnit.Jump) return;

    //1. Determine ALU inputs.
    alu.input1 = regfile.regs[IDEX.ins.rs]; //r1 is always element in register file with index curIns.rs
    alu.input2 = mux(regfile.regs[IDEX.ins.rt], IDEX.ins.signextimm, controlUnit.ALUsrc); //ALU unit input mux
    printf("Alu inputs: %d, %d\n", alu.input1,alu.input2);

    //2. Actual ALU business
    ALUfunct(alu.input1, alu.input2, &err, alu.ALUres); //output can be rs (R-format) or rt (I-format)
    if(err) printf("ERROR: ALU set its error flag\n");

    //3. store ALU result in EXMEM
    EXMEM.ALUres = alu.ALUres;

    //4. pass everything else onward
    EXMEM.ins.instruct = IDEX.ins.instruct;
    EXMEM.ins.opcode = IDEX.ins.opcode;
    EXMEM.ins.rs = IDEX.ins.rs;
    EXMEM.ins.rt = IDEX.ins.rt;
    EXMEM.ins.rd = IDEX.ins.rd;
    EXMEM.ins.shamt = IDEX.ins.shamt;
    EXMEM.ins.funct = IDEX.ins.funct;
    EXMEM.ins.imm = IDEX.ins.imm;
    EXMEM.ins.addr = IDEX.ins.addr;
    EXMEM.PC = IDEX.PC; //TODO: For instructions that change the PC, is this correct? yes since these instructions just set up the IDEX pc, and we need to copy it over

    /*output of ALU will be as follows:
    -1 for error
    0 for successful R-format instruction
    res for address of memory that needs to be loaded into a register or stored in memory.
    */
    /*NOTE THAT:
    currently, there is a bit of a jumble with branch instructions -> these should be occurring in decode, not execute
    */

    //5. Stuff we'll need later for MEM and WB stages
    if (isLoadOrStore(IDEX.ins.opcode) == stores) {
        EXMEM.write_to_mem_reg = IDEX.ins.rt; //the register number that we will write in a storeword instruction
        EXMEM.write_to_mem_val = regfile.regs[IDEX.write_to_mem_reg]; //store value of this reg
        printf("DEBUGG: //////Register number to be stored to: %d, value to be stored: %d\n",EXMEM.write_to_mem_reg,EXMEM.write_to_mem_val);
    }
    if (isLoadOrStore(IDEX.ins.opcode) == Loads) {
        EXMEM.write_reg = IDEX.ins.rt; //register location to write to
        EXMEM.write_addr = EXMEM.ALUres; //memory address to seek
    }

    //8. jr
    if(EXMEM.ins.funct == 0x8) {
        EXMEM.nopFlag = 1;
	amTagalong = 1;
        if (EXMEM.ins.instruct == 0x00000008) {
            EXMEM.EOP_flag = -1; //ends program
	    printf("Program completed like it was SUPPOSED to\n");
        }
        printf("DEBUG:///PC before jump: %d\n",PC);
        BranchAddr = regfile.regs[IDEX.ins.rs];
        controlUnit.Jump = 1; //this needs to be set as the below stages need this information
        printf("DEBUG:///PC after jump: %d and branchaddr %d\n",regfile.regs[EXMEM.ins.rs], BranchAddr);
        return;
    }

}

void MemoryStage(){
    if (EXMEM.nopFlag || controlUnit.Jump) {
        MEMWB.ALUres = EXMEM.ALUres;
        MEMWB.ins.instruct = EXMEM.ins.instruct;
        MEMWB.ins.opcode = EXMEM.ins.opcode;
        MEMWB.ins.rs = EXMEM.ins.rs;
        MEMWB.ins.rt = EXMEM.ins.rt;
        MEMWB.ins.rd = EXMEM.ins.rd;
        MEMWB.ins.shamt = EXMEM.ins.shamt;
        MEMWB.ins.funct = EXMEM.ins.funct;
        MEMWB.ins.imm = EXMEM.ins.imm;
        MEMWB.ins.addr = EXMEM.ins.addr;
        MEMWB.PC = EXMEM.PC; //again, this is the incremented one. not the current inst.
        MEMWB.nopFlag = EXMEM.nopFlag;
        return;
    }

    //1. skip this stage if no memory part is involved
    if (controlUnit.MemRead == 0 && controlUnit.MemWrite == 0 && isLoadOrStore(EXMEM.ins.opcode)) {
            printf("ERROR: Memorystage is not detecting whether to involve mem or not properly.\n");
            exit(0);//TODO: remove this after it isn't necessary to debug
    }

    //2. load word format
    else if (controlUnit.MemRead == 1 && controlUnit.MemWrite == 0) {
        if (isLoadOrStore(EXMEM.ins.opcode) == Loads) { //to double check
            MEMWB.read_data = dataMem[EXMEM.write_addr>>2]; //load result into read_data output of mem
            MEMWB.write_reg = EXMEM.write_reg; //location to write to
            printf("DEBUG: ///memory location: %d, memory value: %d, register location: %d/n",EXMEM.write_addr,MEMWB.read_data,MEMWB.write_reg);
        }
        else  {
            printf("ERROR: Memorystage is not detecting whether to involve mem or not properly at #2.\n");
            return;
        }
    }

    //3. store word format
    else if (controlUnit.MemRead == 0 && controlUnit.MemWrite == 1) {
        if (isLoadOrStore(EXMEM.ins.opcode) == stores) {
            //store();
            dataMem[EXMEM.ALUres>>2] = EXMEM.write_to_mem_val;
            //printf("DEBUG: //////Stored content in Reg number %d which was %d to memory location %d\n",EXMEM.write_to_mem_reg,EXMEM.write_to_mem_val, EXMEM.ALUres);
            printf("DEBUG: ////// dataMem[%d] = %d\n",EXMEM.ALUres,dataMem[EXMEM.ALUres>>2]);
        }
        else  {
            printf("ERROR: Memorystage is not detecting whether to involve mem or not properly at #3 Opcode = %d.\n", EXMEM.ins.opcode);
            return;
        }
    }

    MEMWB.ALUres = EXMEM.ALUres; //store ALU result in EXMEM
    MEMWB.ins.instruct = EXMEM.ins.instruct;
    MEMWB.ins.opcode = EXMEM.ins.opcode;
    MEMWB.ins.rs = EXMEM.ins.rs;
    MEMWB.ins.rt = EXMEM.ins.rt;
    MEMWB.ins.rd = EXMEM.ins.rd;
    MEMWB.ins.shamt = EXMEM.ins.shamt;
    MEMWB.ins.funct = EXMEM.ins.funct;
    MEMWB.ins.imm = EXMEM.ins.imm;
    MEMWB.ins.addr = EXMEM.ins.addr;
    MEMWB.PC = EXMEM.PC; //again, this is the incremented one. not the current inst.
    MEMWB.nopFlag = EXMEM.nopFlag;
    MEMWB.PC = EXMEM.PC;

}
void WritebackStage(){

    if (MEMWB.nopFlag) return;
    if(controlUnit.Jump) return;

    //1. Check need for WB
    if (controlUnit.RegWrite == 0) {
        if (type == stores || type == branches) return;
        else {
            printf("ERROR: WBstage is not detecting whether to involve mem or not properly.\n");
        }
    }

    //2. If we do, in fact, need to write back to registers:
    else if (controlUnit.RegWrite == 1) {
        if (type == Loads || type == Rtype || type == Iform) {
            regfile.writeval = mux(MEMWB.ALUres,MEMWB.read_data, controlUnit.MemToReg); //ALU unit input mux
            //printf("regfile.writeval: %d, from choices of MEMWB.readdata: %d and MEMWB.alures:%d\n",regfile.writeval,MEMWB.read_data,MEMWB.ALUres);
            if (type == Rtype) regfile.writereg = MEMWB.ins.rd;
            else if (type == Iform|| type == Loads) {
                    regfile.writereg = MEMWB.ins.rt;
                    printf("regfile.writereg: %d,MEMWB.ins.rt: %d\n", regfile.writereg, MEMWB.ins.rt);
            }
            regfile.regs[regfile.writereg] = regfile.writeval;
            printf("Value to be written to register %d: %d\n",regfile.writereg, regfile.writeval);

        }
        else { printf("ERROR: WBstage is not detecting whether to involve mem or not properly.\n");
        return;
        }
    }
    else printf("ERROR: WBstage is not detecting whether to involve mem or not properly.\n");


}
