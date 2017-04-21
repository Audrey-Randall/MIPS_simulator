#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "cpuSim.h" //where PC is defined
#include "control.h"
#include "stages.h"
#include <limits.h>

//#define commandline
#define codeblocks
#define additionlogic
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
