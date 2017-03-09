#include "cpuSim.h"

//Initialize structs
void init(){
  controlUnit = {0};
  IFID = {0};
  IDEX = {0};
  EXMEM = {0};
  MEMWB = {0};

}

int parse(char* file) {
  return 0;
}

int32_t ALU(int x, int y) {
  //return x ALUop y
  return 0;
}

int32_t signExt(int32_t offsetField) {
  return 0;
}

int dataMemoryUnit(int32_t addr, int32_t writeData){
  return 0;
}

int32_t mux(int32_t zero, int32_t one, uint8_t ctrl){
  return zero;
}

int main(){

}
