#include "stages.h"
#include "cpuSim.h"
#include "control.h"

void FetchStage(){
  setCurIns();
}

void DecodeStage(){
    //TODO: figure out how/when to set write_data in exmem
    //TODO: check if jump instruc should make rest of stages be nops
  setControls(IFID.ins.opcode); //returns all of the control line values set
  printf("curIns.opcode: %d\n",IFID.ins.opcode);

  if(IFID.ins.opcode == 0x24 || IFID.ins.opcode == 0x25 || IFID.ins.opcode == 0xf || IFID.ins.opcode == 0x23) { //load
    IDEX.write_reg = IFID.ins.rt;
  } else if (IFID.ins.opcode == 0x28 || IFID.ins.opcode == 0x29 || IFID.ins.opcode == 0x2b) { //store
    IDEX.write_reg = -1;
  }

  //Read 2 registers to pass to ALU or read one register and use signExt unit for immediate value
  IDEX.ins.opcode = IFID.ins.opcode;
  IDEX.ins.rs = IFID.ins.rs;
  IDEX.ins.rt = IFID.ins.rt;
  IDEX.ins.rd = IFID.ins.rd;
  IDEX.ins.shamt = IFID.ins.shamt;
  IDEX.ins.funct = IFID.ins.funct;
  IDEX.ins.imm = IFID.ins.imm;
  IDEX.ins.signextimm = signExt(IFID.ins.imm);
  IDEX.ins.addr = IFID.ins.addr;
  IDEX.PC = IFID.PC;
  IDEX.nopFlag = 0;
}

void ExecuteStage(){
    if(IDEX.nopFlag) return;
    int32_t arg1, arg2;
    uint8_t output;
    uint8_t err = 0;

    //TODO: instead of having skipWrite in the pipeline register, put in the control unit

    EXMEM.write_data = IDEX.ins.rt;

    arg1 = regs[IDEX.ins.rs]; //r1 is always element in register file with index curIns.rs
    arg2 = mux(regs[IDEX.ins.rt], signExt(IDEX.ins.imm), controlUnit.ALUsrc); //ALU unit input mux
    ALU(arg1, arg2, &err, output); //output can be rs (R-format) or rt (I-format)
    if(err) printf("ERROR: ALU set its error flag\n");
    /*output of ALU will be as follows:
    -1 for error
    0 for successful R-format instruction
    res for address of memory that needs to be loaded into a register or stored in memory.
    */
    /*NOTE THAT:
    currently, there is a bit of a jumble with branch instructions -> these should be occurring in decode, not execute
    */
    //TODO: copy pipeline reg vals from IDEX to EXMEM
}

void MemoryStage(){
  if(!(EXMEM.nopFlag || EXMEM.skipMem)) {
    if(EXMEM.write_reg == -1) {
      store();
      MEMWB.skipWB = 1;
    } else {
      MEMWB.read_data = load();
      MEMWB.skipWB = 0;
    }
  } else {
    //If you skip the mem stage you always write back?
    MEMWB.skipWB = 0;
  }
  //TODO: set skipWB on store instrucs
  MEMWB.resReg = EXMEM.resReg;
  MEMWB.ALUres = EXMEM.ALUres;
  MEMWB.write_reg = EXMEM.write_reg;
  MEMWB.PC = EXMEM.PC;
  MEMWB.nopFlag = EXMEM.nopFlag;
}
void WritebackStage(){
  if(!(MEMWB.nopFlag||MEMWB.skipWB)) {
    //do writeback
    if(MEMWB.write_reg == -1) {
      //Not a load instruction
      regs[MEMWB.resReg] = MEMWB.write_data;
    } else {
      //Load instruction
      regs[MEMWB.write_reg] = MEMWB.read_data; //read as in past tense of "to read," data read in from mem stage
    }
  }
}
