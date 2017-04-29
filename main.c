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
 #define regsassign
 #define manmem

 #define program1

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
    //progLen = parseInput("Simulation_example.txt");
    progLen = parseInput("Program1File.txt");

#endif // codeblocks
    //Execute program

    #ifdef additionlogic
        //test cases for addition. this will not normally belong in main, this is just where I was brainstorming how to figure out our issue.
        //I will remove this after including this logic with our actual ALU.

        printf("\n random addition problems: \n");
        uint32_t a = 0b10000000000000000000000000000000;
        uint32_t b = 0b10000000000000000000000000000000;
        uint32_t c = a + b; //answer is 0 -> cuts off.
            printf("\n\n random addition: %08X\n",c); //-> cuts off 32-bit range
        //int32_t d = 0b11111111111111111111111111111111;
        //int32_t e = 0b11111111111111111111111111111111;
        int32_t d = 0b01110000000000000000000000000000; //1879048192
        int32_t e = 0b11100000000000000000000000000000; //1610612736
        int32_t f = (d + e);
            printf("\n\n random addition: 0x%d\n",f); //-> cuts off 32-bit range

        uint32_t g = (uint32_t)0b11111111111111111111111111111111;
        uint32_t h = (uint32_t)0b11111111111111111111111111111111;
        //int32_t i = 0x00000000 - (g + h); //this conversion converts the answer from a two's complement number to a normal one.
        uint64_t i = g+h;
        //uint32_t l = g+h;
        //uint32_t i = (uint32_t)((uint32_t)g + (uint32_t)h);

//        if (i <= g || i <= h) {
//                if (i |= 0x100000000) {
//                    printf("\nOVERFLOW\n");
//                }
//        }

        //first if statement is something i found online, second if statement is checking if the 33rd bit is a 1, i.e. if it was too much for the addition to handle
        //since we are dealing with unsigned digits we can also just say if the 64 bit value is equal to 0x1ffffffff i.e. the greatest value that will come out of two 32-bit numberes,
        //then we are in overflow.

        //from my observation, the signed addition seems to account for all of this so we don't need to worry about that!

        printf("\n\n random addition: %u\n",i); //-> cuts off 32-bit range


        printf("\n random addition problems: \n");

        #endif // additionlogic


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
        PC = 140;
        regfile.regs[SP] = 3000;
        regfile.regs[FP] = 3000;
        #endif // program1


        uint32_t endofprog = 0;
    while(PC < progLen) {
        printf("\nmemory[%d]: 0x%08X-------------------------------------------------\n",PC, memory[PC]);
        //for (i=0;i<5;i++) {
        //printf("Reg %d: %d\n",i, regfile.regs[i]);
        //}
        printf("STARTING PC: %d\n",PC);
        FetchStage();
        DecodeStage();
        ExecuteStage();
        if (EXMEM.EOP_flag) break;
        MemoryStage();
        WritebackStage();
        printf("ENDING PC: %d\n",PC);

        //Cleanup
        //PC++;  //if it wasn't messed with elsewhere
        //memset(&curIns, 0, sizeof(curIns));
    }
    printf("Program complete\n");

}
