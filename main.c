#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "cpuSim.h"
#include "control.h"

#define fileread
//#define manualmemory

int main(int argc, char** argv) {
  //Initialize globals
  init();

  #ifdef fileread
  //we are able to take the character by character line-up and convert into a string. problem is that we have to convert
   FILE *infile;
   //int * memory = (char*)malloc(sizeof(int)*6); //address memory -> dynamic allocation
   int fileFlag = -1;

   //check that text file conversion happened properly
   //NOTE: eventually make text file as input to main (argv,argc) so prof can run with his instruction set
   while(fileFlag) {
        fileFlag = textFileConversion(infile);

        //granted that file opened okay, we continue to our stages.
        while(PC < idx) {
            printf("memory[%d]: %d-------------------------------------------------\n",PC, memory[PC]);
            FetchStage();
            DecodeStage();
            ExecuteStage();
            MemoryStage();
            WritebackStage();

            //Cleanup
            PC++;  //if it wasn't messed with elsewhere
            //memset(&curIns, 0, sizeof(curIns));
        }
   }

   #endif // fileread

   PC = 0;

  //Read instructions from file into memory array
  /*if(parse("Simulation_example.txt")) {
    printf("Cannot read machine code\n");
    exit(1);
  }*/
  #ifdef DEBUG
  /*memory[0] = 0x2116022;
  memory[1] = 0x8C0C0;
  memory[2] = 0x15F7F0;
  memory[3] = 0x2088009c;
  memory[4] = 0xCA9B987;*/
  #endif

  #ifdef manualmemory
  memory[0] = 0x2116022; //34693154, 000000 10000 10001 01100 00000 100010
  memory[1] = 0x8C0C0; //573632, 000000 00000 01000 11000 00011 000000
  memory[2] = 0x15F7F0; //1439728, 000000 00000 10101 11110 11111 110000
  memory[3] = 0x2088009c; //545783964, 001000 00100 01000 0000000010011100
  memory[4] = 0xCA9B987; //40648736, 000000 10011 01100 01000 00000 100000
  #endif // manualmemory

}
