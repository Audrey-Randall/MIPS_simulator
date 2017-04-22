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

    EXMEM.write_data = IDEX.ins.rt;
    
    arg1 = regs[IDEX.ins.rs]; //r1 is always element in register file with index curIns.rs
    arg2 = mux(regs[IDEX.ins.rt], signExt(IDEX.ins.imm), controlUnit.ALUsrc); //ALU unit input mux
    ALU(arg1, arg2, &err, output); //output can be rs (R-format) or rt (I-format)
    /*output of ALU will be as follows:
    -1 for error
    0 for successful R-format instruction
    res for address of memory that needs to be loaded into a register or stored in memory.
    */
    /*NOTE THAT:
    currently, there is a bit of a jumble with branch instructions -> these should be occurring in decode, not execute
    */
}
void MemoryStage(){
  if(EXMEM.nopFlag) return;
}
void WritebackStage(){
  if(MEMWB.nopFlag) return;
}
