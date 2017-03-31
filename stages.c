#include "stages.h"
#include "cpuSim.h"
#include "control.h"

void FetchStage(){
  setCurIns();
}

void DecodeStage(){
  setControls(curIns.opcode); //returns all of the control line values set
  //Read 2 registers to pass to ALU or read one register and use signExt unit for immediate value
  int arg1, arg2, output, err;
  arg1 = regs[curIns.rs]; //r1 is always element in register file with index curIns.rs
  arg2 = mux(signExt(curIns.imm), regs[curIns.rt], controlUnit.ALUsrc); //ALU unit input mux
  err = ALU(arg1, arg2, err, output);
}
void ExecuteStage(){

}
void MemoryStage(){

}
void WritebackStage(){

}

