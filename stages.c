#include "stages.h"
#include "cpuSim.h"
#include "control.h"

void FetchStage(){
  setCurIns();
}

void DecodeStage(){
    //TODO: figure out how/when to set write_data in exmem
  setControls(curIns.opcode); //returns all of the control line values set
      printf("curIns.opcode: %d\n",curIns.opcode);

  //Read 2 registers to pass to ALU or read one register and use signExt unit for immediate value
  if (InstructionType = Itype) {
    curIns.signextimm = signExt(curIns.imm);
  }
}
void ExecuteStage(){
    int arg1, arg2, output;
    output = regs[curIns.rd];
    int err = 0;
    while (!err) {
        arg1 = regs[curIns.rs]; //r1 is always element in register file with index curIns.rs
        arg2 = mux(regs[curIns.rt], signExt(curIns.imm), controlUnit.ALUsrc); //ALU unit input mux
        ALU(arg1, arg2, curIns.imm, &err, output); //output can be rs (R-format) or rt (I-format)
        /*output of ALU will be as follows:
        -1 for error
        0 for successful R-format instruction
        res for address of memory that needs to be loaded into a register or stored in memory.
        */
        /*NOTE THAT:
        currently, there is a bit of a jumble with branch instructions -> these should be occurring in decode, not execute
        */
        return;
    }
}
void MemoryStage(){

}
void WritebackStage(){

}
