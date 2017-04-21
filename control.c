#define OPCODE_R 0b000000 //0x00
#define OPCODE_LW 0b100011 //0x23
#define OPCODE_SW 0b101011 //0x2b
#define OPCODE_BEQ 0b000100 //0x4



#include "control.h"
//#include "cpuSim.h"

//Figure 4.22 tells how to implement this
int setControls(uint8_t opcode){
  switch(opcode){
    case OPCODE_R:
      controlUnit.RegDst = 1;
      controlUnit.ALUsrc = 0;
      controlUnit.MemToReg = 0;
      controlUnit.RegWrite = 1;
      controlUnit.MemRead = 0;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 0;
      controlUnit.ALUop = 0b10;
      printf("R-format\n");
      break;
    case OPCODE_LW:
      controlUnit.RegDst = 0;
      controlUnit.ALUsrc = 1;
      controlUnit.MemToReg = 1;
      controlUnit.RegWrite = 1;
      controlUnit.MemRead = 1;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 0;
      controlUnit.ALUop = 0b00;
      printf("LW-format\n");
      break;
    case OPCODE_SW:
      controlUnit.RegDst = 0;  //X in diagram in book
      controlUnit.ALUsrc = 1;
      controlUnit.MemToReg = 0;  //X in book
      controlUnit.RegWrite = 0;
      controlUnit.MemRead = 0;
      controlUnit.MemWrite = 1;
      controlUnit.Branch = 0;
      controlUnit.ALUop = 0b00;
      printf("SW-format\n");
      break;
    case OPCODE_BEQ:
      controlUnit.RegDst = 0;  //X in book
      controlUnit.ALUsrc = 0;
      controlUnit.MemToReg = 0;  //X in book
      controlUnit.RegWrite = 0;
      controlUnit.MemRead = 0;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 1;
      controlUnit.ALUop = 0b01;
      printf("BEQ-format\n");
      break;
    default:printf("Control lines not set.\n");
      return -1;
  }
  return 0;
}

//#define printinstructionmemory

int textFileConversion(FILE *fp) {
    int i;
    char input[1000]; //to store each char into an array (to make it a string)
    char * hexarray[8]; //array of strings
    int c;
    uint32_t number;
    fp = fopen("Simulation_example.txt","r");
    if (fp == NULL) {
        printf("cannot open file.\n");
        return -1;
    }

    //store each character into a string, print each line
    else {
        do
        {
            if(fgets(input, sizeof(input), fp) == NULL) {
                perror("ERROR: fgets in textFileConversion()");
                exit(1);
            }

            number = strtol(input, NULL, 0); //converts hex string number to int
            //printf(" NUMBER: %ld",number);
            hexarray[idx] = input;
            memory[idx] = number;
            //printf(" MEMORY INDEX: %ld\n",memory[idx]);
            idx++; //counts how many instructions are in text file.
        }
        while ((c = getc(fp)) != ',' && c != EOF);
    }
    fclose(fp);

    #ifdef printinstructionmemory
    //print integer array
    for(i=0; i<idx-1; i++) {
        printf("element %d in dec: %d\n",i,memory[i]);
    }

    #endif // printinstructionmemory

    return 0;
}
