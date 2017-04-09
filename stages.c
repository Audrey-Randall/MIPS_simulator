#include "stages.h"
#include "cpuSim.h"
#include "control.h"

void FetchStage(){
  init();
  //change this
  setCurIns();
}

void DecodeStage(){
  setControls(curIns.opcode); //returns all of the control line values set
      printf("curIns.opcode: %d\n",curIns.opcode);

  //Read 2 registers to pass to ALU or read one register and use signExt unit for immediate value
  if (InstructionType = Itype) {
    curIns.signextimm = signExt(curIns.imm);
  }
}
void ExecuteStage(){
    int arg1, arg2, output;
    int err = 0;
    while (!err) {
        arg1 = regs[curIns.rs]; //r1 is always element in register file with index curIns.rs
        arg2 = mux(regs[curIns.rt], signExt(curIns.imm), controlUnit.ALUsrc); //ALU unit input mux
        err = ALU(arg1, arg2, err, output); //output can be rs (R-format) or rt (I-format)
        return;
    }
}
void MemoryStage(){

}
void WritebackStage(){

}
