#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "cpuSim.h"
#include "control.h"
#include "stages.h"

//#define manualmemory

int main(int argc, char** argv) {
  int progLen;
  //Check usage
  if(argc < 2) {
    printf("USAGE: ./cpuSim <program to read>\n");
    exit(0);
  }
  //Initialize globals
  init();
  //Read program file into memory
  progLen = parseInput(argv[1]);

  //Execute program
  while(PC < progLen) {
    printf("memory[%d]: 0x%08X-------------------------------------------------\n",PC, memory[PC]);
    FetchStage();
    DecodeStage();
    ExecuteStage();
    MemoryStage();
    WritebackStage();

    //Cleanup
    PC++;  //if it wasn't messed with elsewhere
    //memset(&curIns, 0, sizeof(curIns));
  }

  #ifdef manualmemory
  memory[0] = 0x2116022; //34693154, 000000 10000 10001 01100 00000 100010
  memory[1] = 0x8C0C0; //573632, 000000 00000 01000 11000 00011 000000
  memory[2] = 0x15F7F0; //1439728, 000000 00000 10101 11110 11111 110000
  memory[3] = 0x2088009c; //545783964, 001000 00100 01000 0000000010011100
  memory[4] = 0xCA9B987; //40648736, 000000 10011 01100 01000 00000 100000
  #endif // manualmemory

}
