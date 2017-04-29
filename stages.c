#include "stages.h"
#include "cpuSim.h"
#include "control.h"

void FetchStage(){

    //branch control logic
    if (controlUnit.WasBranch && !controlUnit.ShouldExec) {
        controlUnit.ShouldExec = 1;
    }

    else if (controlUnit.WasBranch && controlUnit.ShouldExec) {
        controlUnit.WasBranch = 0;
        controlUnit.ShouldExec = 0;
    }

    //1. Instruction is read from memory using the address in the PC
    ci = memory[PC];  //current instruction
    //branch PC logic

    //2. Instruction itself is stored into IFID register.
    IFID.ins.instruct = ci;

    //3. PC incremented, stored to pipeline, for need in things like branch instruction
    PC++;
    IFID.PC = PC;

    //printf("IFID.PC: %d\n",IFID.PC);
}

void DecodeStage() {


    //1. Instruction decode. Register numbers, imm fields, etc. supplied.
    setCurIns(ci);

    //branch variables
    uint8_t zero=0;
    int32_t v1 = regfile.regs[IFID.ins.rs];
    int32_t v2 = regfile.regs[IFID.ins.rt];
    int32_t signext = signExt(IFID.ins.imm);

    //TODO: check if jump instruc should make rest of stages be nops


    //2. Based off of this decoding, set control lines
    setControls(IFID.ins.opcode);

    //3. Register numbers fed into register file.
    regfile.readreg1 = IFID.ins.rs;
    regfile.readreg2 = mux(IFID.ins.rt, IFID.ins.rd, controlUnit.RegDst);
    //printf("decode stage: readreg1 = %d, readreg2 = %d\n",regfile.readreg1, regfile.readreg2);

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
    IDEX.PC = IFID.PC; //again, this is the incremented one. not the current inst.
    IDEX.nopFlag = 0;

    //7. to use later for stores
    if (type == stores) {
        IDEX.write_to_mem_reg = IFID.ins.rt; //the register number that we will write in a storeword instruction
        IDEX.write_to_mem_val = regfile.regs[IDEX.write_to_mem_reg]; //store value of this reg
    }

    //9. Jump / Branches should occur in decode. Add here.
    if (controlUnit.Jump) {
        if (IDEX.ins.opcode == 0x2) {

            //an arbitrary example of some address "var"
            /*
            uint32_t var = 0x22222222;
            uint32_t temp1 = (uint32_t)(var & 0xf0000000);
            printf("IFID.ins.addr before editing: %08X\n",IFID.ins.addr);
            IFID.ins.addr = 0x000000ff;
            uint32_t temp2 = (IFID.ins.addr << 2);
            uint32_t temp3 = (0x00);
            uint32_t finaladdress = (temp1 | temp2 | temp3);
            printf("temp1: %08X, temp2: %08X, temp3: %08X, finaladd: %08X\n",temp1,temp2,temp3,finaladdress);
            */
            printf("IFID.ins.addr before editing: %08X\n",IFID.ins.addr);
            uint32_t temp1 = (uint32_t)(IFID.ins.addr & 0xf0000000);
            uint32_t temp2 = (IFID.ins.addr << 2);
            uint32_t temp3 = (uint32_t)(0x00);
            uint32_t finaladdress = (temp1 | temp2 | temp3);
            printf("temp1: %08X, temp2: %08X, temp3: %08X, finaladd: %08X\n",temp1,temp2,temp3,finaladdress);
            PC = finaladdress; //jump to this address
            IDEX.nopFlag = 1; //so we skip over all the next stages
            printf("JUMP!\n");
            //IDEX.EOP_flag = -1; //ends program - REMOVE IN ACTUAL IMPLEMENTATION

        }
        else if (IDEX.ins.opcode = 0x3) {
            printf("IFID.ins.addr before editing: %08X\n",IFID.ins.addr);
            uint32_t temp1 = (uint32_t)(IFID.ins.addr & 0xf0000000);
            uint32_t temp2 = (IFID.ins.addr << 2);
            uint32_t temp3 = (uint32_t)(0x00);
            uint32_t finaladdress = (temp1 | temp2 | temp3);
            printf("temp1: %08X, temp2: %08X, temp3: %08X, finaladd: %08X\n",temp1,temp2,temp3,finaladdress);
            regfile.regs[RA] = finaladdress;
            PC = finaladdress;
            printf("JAL!\n");
            printf("ENDING PC: %d\n",PC);
            //IDEX.EOP_flag = -1; //ends program - REMOVE IN ACTUAL IMPLEMENTATION
        }
        return;
    }

    //branch logic
    if (controlUnit.Branch || controlUnit.WasBranch) {
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
            printf("BLEZ:v1: %d\n",v1);
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


}

void ExecuteStage(){
    int i;
    uint8_t err = 0;

    //check for nop
    if(IDEX.nopFlag) return;
    if(controlUnit.Jump) return;

    //1. Determine ALU inputs.
    alu.input1 = regfile.regs[IDEX.ins.rs]; //r1 is always element in register file with index curIns.rs
    alu.input2 = mux(IDEX.regdata2, IDEX.ins.signextimm, controlUnit.ALUsrc); //ALU unit input mux
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
    if (type == stores) {
        EXMEM.write_to_mem_reg = IDEX.ins.rt; //the register number that we will write in a storeword instruction
        EXMEM.write_to_mem_val = regfile.regs[IDEX.write_to_mem_reg]; //store value of this reg
        printf("DEBUGG: //////Register number to be stored to: %d, value to be stored: %d\n",EXMEM.write_to_mem_reg,EXMEM.write_to_mem_val);
    }
    if (type == Loads) {
        EXMEM.write_reg = IDEX.ins.rt; //register location to write to
        EXMEM.write_addr = EXMEM.ALUres; //memory address to seek
    }

    //8. jr
    if(EXMEM.ins.funct == 0x8) {
        if (EXMEM.ins.instruct == 0x00000008) {
            EXMEM.nopFlag = 1;
            EXMEM.EOP_flag = -1; //ends program
            return;
        }
        printf("DEBUG:///PC before jump: %d\n",PC);
        EXMEM.PC = regfile.regs[IDEX.ins.rs];
        controlUnit.Jump = 1; //this needs to be set as the below stages need this information
        PC = EXMEM.PC; //this is what is causing the infinite loop
        printf("DEBUG:///PC after jump: %d\n",regfile.regs[EXMEM.ins.rs]);
        return;
    }

}

void MemoryStage(){

    if(controlUnit.Jump) return;
    if (EXMEM.nopFlag) return;

    //check for noop
    if (EXMEM.nopFlag) {
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
    if (controlUnit.MemRead == 0 && controlUnit.MemWrite == 0) {
        if (type == Rform || type == branches || type == jumps || type == Iform) {
            printf("nomem\n"); //to double check
        }
        else  {
            printf("ERROR: Memorystage is not detecting whether to involve mem or not properly.\n");
            return;
        }
    }

    //2. load word format
    else if (controlUnit.MemRead == 1 && controlUnit.MemWrite == 0) {
        if (type == Loads) { //to double check
            MEMWB.read_data = dataMem[EXMEM.write_addr]; //load result into read_data output of mem
            MEMWB.write_reg = EXMEM.write_reg; //location to write to
            printf("DEBUG: ///memory location: %d, memory value: %d, register location: %d/n",EXMEM.write_addr,MEMWB.read_data,MEMWB.write_reg);
        }
        else  {
            printf("ERROR: Memorystage is not detecting whether to involve mem or not properly.\n");
            return;
        }
    }

    //3. store word format
    else if (controlUnit.MemRead == 0 && controlUnit.MemWrite == 1) {
        if (type == stores) {
            //store();
            dataMem[EXMEM.ALUres] = EXMEM.write_to_mem_val;
            //printf("DEBUG: //////Stored content in Reg number %d which was %d to memory location %d\n",EXMEM.write_to_mem_reg,EXMEM.write_to_mem_val, EXMEM.ALUres);
            printf("DEBUG: ////// dataMem[%d] = %d\n",EXMEM.ALUres,dataMem[EXMEM.ALUres]);
        }
        else  {
            printf("ERROR: Memorystage is not detecting whether to involve mem or not properly.\n");
            return;
        }
    }
    else printf("ERROR: Memorystage is not detecting whether to involve mem or not properly.\n");

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
    MEMWB.PC = EXMEM.PC; //TODO: For instructions that change the PC, is this correct? yes since these instructions just set up the IDEX pc, and we need to copy it over

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
