#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "cpuSim.h"
//#include "control.h"

FILE *file;

int parse(char* filename){
  int i,j;
  printf("Starting simulation\n");
  init();
//printf("Sign ext of 0xffff is %d", signExt(0xffff));
  char hexarray[11];
  char info;
  uint8_t counter = 0;
  file = fopen(filename,"r"); //r is read-only option
  if(file==NULL){
    perror("ERROR: Cannot open file");
    return -1;
  }
  if (file) {
      int chr = 0;
      while(1) {
          if( feof(file) ) break;
          info = fgetc(file);
              chr++;
          }
          for (j=0; j<8; j++) {
              printf("%c",hexarray[j]);
          }
          printf("\n");
  }
  //printf("%c", file);
  fclose(file);
  return 0;
}

int main(int argc, char** argv) {
  //Initialize globals
  init();

  //Read instructions from file into memory array
  /*if(parse("Simulation_example.txt")) {
    printf("Cannot read machine code\n");
    exit(1);
  }*/
  #ifdef DEBUG
  memory[0] = 0x2116022;
  memory[1] = 0x8C0C0;
  memory[2] = 0x15F7F0;
  memory[3] = 0x2088009c;
  memory[4] = 0xCA9B987;
  #endif

  while(PC < 5) {
    FetchStage();
    DecodeStage();
    ExecuteStage();
    MemoryStage();
    WritebackStage();

    //Cleanup
    PC++;  //if it wasn't messed with elsewhere
    memset(&curIns, 0, sizeof(curIns));
  }
}
