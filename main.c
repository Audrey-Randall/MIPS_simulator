#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "cpuSim.h" //where PC is defined
#include "control.h"
#include "stages.h"
#include <limits.h>



//#define commandline
#define codeblocks
//#define additionlogic
 //#define memorypeek
 //#define regsassign
 //#define manmem

// #define program1
#define cache


#ifdef commandline
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

#endif // commandline

#ifdef codeblocks
int main() {
    //Initialize globals
    init();
    int progLen;


    //Read program file into memory
    progLen = parseInput("Simulation_example.txt");
    //progLen returns the input, parseInput actually goes through and stores stuff to memory.
    PC = 1; //for sample program.
    //progLen = parseInput("Program1File.txt");

#endif // codeblocks
    //Execute program


        #ifdef memorypeek
        uint64_t i = 0;
        for (i=0; i<MEMSIZE; i++) {
            printf("%d: %d\n",i, dataMem[i]);
        }
        #endif // memorypeek

        #ifdef regsassign
        uint32_t i;
        for (i=0; i<32; i++) {
            regfile.regs[i] = -2;
            printf("Reg %d: %d\n",i, regfile.regs[i]);
        }
        #endif // regsassign
        #ifdef manmem
        uint32_t j;
        for (j=0; j<MEMSIZE; j++) {
            dataMem[j] = j;
            //printf("MEM %d: %d\n",i, regfile.regs[i]);
        }
        #endif // manmem

         #ifdef program1
        PC = 140*4;
        regfile.regs[SP] = 3000;
        regfile.regs[FP] = 3000;
        #endif // program1


        uint32_t i=0;
        uint32_t endofprog = 0;

        /*
    int k,j;
    for (j=0; j<MEMSIZE; j++) {
            printf("%d: %d\n",j, memory[j]);
        }
*/

    //just for reference.
/*    pipeCtl.fetch = 0;
    pipeCtl.decode = 0;
    pipeCtl.execute = 0;
    pipeCtl.memory = 0;
    pipeCtl.writeback = 0; */

    //printf("PC:%d",PC);


    #ifdef cache

  memset(writeBuffer.data, 0, BLOCKSIZE); //TODO: transfer this into init
  int l;
  for(l = 0; l < ICACHESIZE/(BLOCKSIZE*4); l++) {
    memset(ICache[l].data, 0, BLOCKSIZE);
  }
  for(l = 0; l < DCACHESIZE/(BLOCKSIZE*4); l++) {
    memset(DCache[l].data, 0, BLOCKSIZE);
  }
  //For 4 blocks, 16 words each. So tag len = 32-8 = 24
  //Tag = 0x81234 block is 1 word is 5 byte is 2
  int addr1 = 0x08123456;
  //tag = 123498 block 1 word 13 byte 2
  int addr2 = 0x12349876;
  //tag = afbf00 block 0 word 4 byte 0
  int addr3 = 0xafbf0010;
  //tag = 208800 block 2 word 7 byte 0
  int addr4 = 0x2088009c;

  //Perform tests
  //testAddr(); //test 0
  //testRW(); //test 1, 2

    #endif // cache

    /*
    while(PC != 0) {
         memset((void*)&controlUnit, 0, sizeof(controlUnit));
        FetchStage();
        DecodeStage();
        ExecuteStage();
        if (EXMEM.EOP_flag) break;
        MemoryStage();
        WritebackStage();
        printf("ENDING PC: %d\n",PC);

        if(PC>>2 > 483){
            printf("PC too high!");
	    }

	    //track state of registers as we go through program.
        //for (i=0; i<31; i++) {
          //  printf("Reg %d: %d ",i, regfile.regs[i]);
        //}
        tinstructioncount++;
        printf("\ntinstructioncount: %d\n",tinstructioncount);

        //Cleanup
        //PC++;  //if it wasn't messed with elsewhere
        //memset(&curIns, 0, sizeof(curIns));
    }

    printf("Loop exited\n");
    //uint32_t k,j;
    for (k=0; k<32; k++) {
        printf("Reg %d: %d",k, regfile.regs[k]);
    }
    for (j=0; i=j<MEMSIZE; j++) {
            printf("%d: %d\n",j, memory[j]);
        }
        printf("\ntinstructioncount: %d\n",tinstructioncount);

        */


        //1. Instruction is read from memory using the address in the PC

            PC = 1;

            //load first 5 instructions to start us off

//            uint32_t ci = memory[PC>>2];  //current instruction
 //           printf("ci:%d\n",ci);



   //Every stage needs to move to the next stage
  //If you're at stage 5, go to stage 1 and increment the instruc you're working with by 5 for 5-stage pipeline
  //{stage, instruc}

Ticker ticker1;
Ticker ticker2;
Ticker ticker3;
Ticker ticker4;
Ticker ticker5;
ticker1.stage = 0;
ticker2.stage = 0;
ticker3.stage = 0;
ticker4.stage = 0;
ticker5.stage = 0;
int cycle = 0;
PC = 0;


//Initialize sequence
ticker1.instruc = memory[PC];
ticker2.instruc = memory[PC+1];
ticker3.instruc = memory[PC+2];
ticker4.instruc = memory[PC+3];
ticker5.instruc = memory[PC+4];
//PC = PC+4;
//printf("First 5 instructions: %x,%x,%x,%x,%x\n\n",ticker1.instruc,ticker2.instruc,t3.instruc,t4.instruc,t5.instruc);


  //printf("INIT: ticker1.ins: 0x%x, ticker2.ins: 0x%x, ticker3.ins: 0x%x, ticker4.ins: 0x%x ,ticker5.ins: 0x%x\n",ticker1.instruc,ticker2.instruc,ticker3.instruc,ticker4.instruc,ticker5.instruc);


  //First instruction
      printf("cycle = %d\n", cycle);
  FetchStage(IFIDShadowIns1,ticker1.instruc); //sets IFID,shadowins1 to IFID.
  printf("\n Here, in our program, we see that there was actually nothing written to the shadow register.\nIFIDShowIns1 = 0x%x\n\n",IFIDShadowIns1.ins.instruct);
  ticker1.stage++;
  cycle++;
  //printf("CYCLE1: ticker1.instruc: %x\n",ticker1.instruc);
  //printf("ticker1.stage: %d, ticker2.stage: %d, ticker3.stage: %d, ticker4.stage: %d ,ticker5.stage: %d\n",ticker1.stage,ticker2.stage,ticker3.stage,ticker4.stage,ticker5.stage);
  PC++;
  printf("\n\n");

  //Second instruction
        printf("cycle = %d\n", cycle);
  FetchStage(IFIDShadowIns2, ticker2.instruc); //sets IFID again
  DecodeStage(IFIDShadowIns1, ticker1.instruc); //ticker1's timeline. need IFIDShadow
  ticker1.stage++;
  ticker2.stage++;
  cycle++;
  PC++;
  //printf("CYCLE2: ticker2.instruc: %x\n",ticker2.instruc);
  //printf("ticker1.stage: %d, ticker2.stage: %d, ticker3.stage: %d, ticker4.stage: %d ,ticker5.stage: %d\n",ticker1.stage,ticker2.stage,ticker3.stage,ticker4.stage,ticker5.stage);

  printf("\n\n");

  //Third instruction
        printf("cycle = %d\n", cycle);
  FetchStage(IFIDShadowIns3,ticker3.instruc);
  DecodeStage(IFIDShadowIns2,ticker2.instruc); //ticker1's timeline
  ExecuteStage(IFIDShadowIns1,ticker1.instruc);
  ticker1.stage++;
  ticker2.stage++;
  ticker3.stage++;
  cycle++;
  PC++;
  //printf("CYCLE3: ticker3.instruc: %x\n",ticker3.instruc);
  //printf("ticker1.stage: %d, ticker2.stage: %d, ticker3.stage: %d, ticker4.stage: %d ,ticker5.stage: %d\n",ticker1.stage,ticker2.stage,ticker3.stage,ticker4.stage,ticker5.stage);

  printf("\n\n");
  //Fourth instruction
        printf("cycle = %d\n", cycle);
  FetchStage(IFIDShadowIns4,ticker4.instruc);
  DecodeStage(IFIDShadowIns3,ticker3.instruc); //ticker1's timeline
  ExecuteStage(IFIDShadowIns2,ticker2.instruc);
  MemoryStage(IFIDShadowIns1,ticker1.instruc);
  ticker1.stage++;
  ticker2.stage++;
  ticker3.stage++;
  ticker4.stage++;
  cycle++;
  PC++;

  //printf("CYCLE4: ticker4.instruc: %x\n",ticker4.instruc);
//printf("ticker1.stage: %d, ticker2.stage: %d, ticker3.stage: %d, ticker4.stage: %d ,ticker5.stage: %d\n",ticker1.stage,ticker2.stage,ticker3.stage,ticker4.stage,ticker5.stage);
 printf("\n\n");

// PC=PC+5;

    while(PC < progLen) {
    printf("cycle = %d\n", cycle);


    switch(ticker1.stage) {

      case 0:
        ticker1.instruc = memory[PC];
        FetchStage(IFIDShadowIns1,ticker1.instruc);
        DecodeStage(IFIDShadowIns5,ticker5.instruc);
        ExecuteStage(IFIDShadowIns4,ticker4.instruc);
        MemoryStage(IFIDShadowIns3,ticker3.instruc);
        WritebackStage(IFIDShadowIns2,ticker2.instruc);
        incrementTickers(PC,&ticker1, &ticker2, &ticker3, &ticker4, &ticker5);
        PC++;
        break;
      case 1:
        ticker2.instruc = memory[PC];
        FetchStage(IFIDShadowIns2,ticker2.instruc);
        DecodeStage(IFIDShadowIns1,ticker1.instruc);
        ExecuteStage(IFIDShadowIns5,ticker5.instruc);
        MemoryStage(IFIDShadowIns4,ticker4.instruc);
        WritebackStage(IFIDShadowIns3,ticker3.instruc);
        incrementTickers(PC,&ticker1, &ticker2, &ticker3, &ticker4, &ticker5);
        PC++;
        break;
      case 2:
        ticker3.instruc = memory[PC];
        FetchStage(IFIDShadowIns3,ticker3.instruc); //ticker3
        DecodeStage(IFIDShadowIns2,ticker2.instruc); //ticker2
        ExecuteStage(IFIDShadowIns1,ticker1.instruc); //ticker1
        MemoryStage(IFIDShadowIns5,ticker5.instruc); //ticker5
        WritebackStage(IFIDShadowIns4,ticker4.instruc); //ticker4
        incrementTickers(PC,&ticker1, &ticker2, &ticker3, &ticker4, &ticker5);
        PC++;
        break;
      case 3:
        ticker4.instruc = memory[PC];
        FetchStage(IFIDShadowIns4,ticker4.instruc); //ticker4
        DecodeStage(IFIDShadowIns3,ticker3.instruc); //ticker3
        ExecuteStage(IFIDShadowIns2,ticker2.instruc); //ticker2
        MemoryStage(IFIDShadowIns1,ticker1.instruc); //ticker1
        WritebackStage(IFIDShadowIns5,ticker5.instruc); //ticker5
        incrementTickers(PC,&ticker1, &ticker2, &ticker3, &ticker4, &ticker5);
        PC++;
        break;
      case 4:
        ticker5.instruc = memory[PC];
        FetchStage(IFIDShadowIns5,ticker5.instruc);
        DecodeStage(IFIDShadowIns4,ticker4.instruc);
        ExecuteStage(IFIDShadowIns3,ticker3.instruc);
        MemoryStage(IFIDShadowIns2,ticker2.instruc);
        WritebackStage(IFIDShadowIns1,ticker1.instruc);
        incrementTickers(PC,&ticker1, &ticker2, &ticker3, &ticker4, &ticker5);
        PC++;
        break;
    }

    printf("\n\n");
    cycle++;



//            tick(cycle, ticker1,ticker2,ticker3,ticker4,ticker5);
            //PC++;
            //ticker1.instruc = memory[PC];
        }

}
