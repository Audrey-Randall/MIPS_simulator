
#include "control.h"
//#include "cpuSim.h"


//Figure 4.22 tells how to implement this
int setControls(uint16_t opcode){

    //R-formats
    if (opcode == OPCODE_R) {
      controlUnit.RegDst = 1;
      controlUnit.ALUsrc = 0;
      controlUnit.MemToReg = 0;
      controlUnit.RegWrite = 1;
      controlUnit.MemRead = 0;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 0;
      controlUnit.ALUop = 0b10;
      controlUnit.Jump = 0;
      printf("R-format\n");
      type = Rform;
    }


    //Loads
    else if (opcode == OPCODE_LW) {
      controlUnit.RegDst = 0;
      controlUnit.ALUsrc = 1;
      controlUnit.MemToReg = 1;
      controlUnit.RegWrite = 1;
      controlUnit.MemRead = 1;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 0;
      controlUnit.ALUop = 0b00;
      controlUnit.Jump = 0;
      printf("LW-format\n");
      type = Loads;
    }
    else if (opcode == OPCODE_LB) {
      controlUnit.RegDst = 0;
      controlUnit.ALUsrc = 1;
      controlUnit.MemToReg = 1;
      controlUnit.RegWrite = 1;
      controlUnit.MemRead = 1;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 0;
      controlUnit.ALUop = 0b00;
      controlUnit.Jump = 0;
      printf("LW-format\n");
      type = Loads;
    }
    else if (opcode == OPCODE_LBU) {
      controlUnit.RegDst = 0;
      controlUnit.ALUsrc = 1;
      controlUnit.MemToReg = 1;
      controlUnit.RegWrite = 1;
      controlUnit.MemRead = 1;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 0;
      controlUnit.ALUop = 0b00;
      controlUnit.Jump = 0;
      printf("LW-format\n");
      type = Loads;
    }
    else if (opcode == OPCODE_LHU) {
      controlUnit.RegDst = 0;
      controlUnit.ALUsrc = 1;
      controlUnit.MemToReg = 1;
      controlUnit.RegWrite = 1;
      controlUnit.MemRead = 1;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 0;
      controlUnit.ALUop = 0b00;
      controlUnit.Jump = 0;
      printf("LW-format\n");
      type = Loads;
    }
    else if (opcode == OPCODE_LUI) {
      controlUnit.RegDst = 0;
      controlUnit.ALUsrc = 1;
      controlUnit.MemToReg = 1;
      controlUnit.RegWrite = 1;
      controlUnit.MemRead = 1;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 0;
      controlUnit.ALUop = 0b00;
      controlUnit.Jump = 0;
      printf("LW-format\n");
      type = Loads;
    }

    //stores
    else if (opcode == OPCODE_SW) {
      controlUnit.RegDst = 0;  //X in diagram in book
      controlUnit.ALUsrc = 1;
      controlUnit.MemToReg = 0;  //X in book
      controlUnit.RegWrite = 0;
      controlUnit.MemRead = 0;
      controlUnit.MemWrite = 1;
      controlUnit.Branch = 0;
      controlUnit.ALUop = 0b00;
      controlUnit.Jump = 0;
      printf("SW-format\n");
      type = stores;
    }
    else if (opcode == OPCODE_SB) {
      controlUnit.RegDst = 0;  //X in diagram in book
      controlUnit.ALUsrc = 1;
      controlUnit.MemToReg = 0;  //X in book
      controlUnit.RegWrite = 0;
      controlUnit.MemRead = 0;
      controlUnit.MemWrite = 1;
      controlUnit.Branch = 0;
      controlUnit.ALUop = 0b00;
      controlUnit.Jump = 0;
      printf("SW-format\n");
      type = stores;
    }
    else if (opcode == OPCODE_SH) {
      controlUnit.RegDst = 0;  //X in diagram in book
      controlUnit.ALUsrc = 1;
      controlUnit.MemToReg = 0;  //X in book
      controlUnit.RegWrite = 0;
      controlUnit.MemRead = 0;
      controlUnit.MemWrite = 1;
      controlUnit.Branch = 0;
      controlUnit.ALUop = 0b00;
      controlUnit.Jump = 0;
      printf("SW-format\n");
      type = stores;
    }

    //Branch-format
    else if (opcode == OPCODE_BEQ) {
      controlUnit.RegDst = 0;  //X in book
      controlUnit.ALUsrc = 0;
      controlUnit.MemToReg = 0;  //X in book
      controlUnit.RegWrite = 0;
      controlUnit.MemRead = 0;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 1;
      controlUnit.ALUop = 0b01;
      controlUnit.Jump = 0;
      //printf("BEQ-format\n");

      type = branches;
    }
    else if (opcode == OPCODE_BGTZ) {
      controlUnit.RegDst = 0;  //X in book
      controlUnit.ALUsrc = 0;
      controlUnit.MemToReg = 0;  //X in book
      controlUnit.RegWrite = 0;
      controlUnit.MemRead = 0;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 1;
      controlUnit.ALUop = 0b01;
      controlUnit.Jump = 0;
      //printf("BEQ-format\n");
      type = branches;
    }
    else if (opcode == OPCODE_BLEZ) {
      controlUnit.RegDst = 0;  //X in book
      controlUnit.ALUsrc = 0;
      controlUnit.MemToReg = 0;  //X in book
      controlUnit.RegWrite = 0;
      controlUnit.MemRead = 0;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 1;
      controlUnit.ALUop = 0b01;
      controlUnit.Jump = 0;
      //printf("BEQ-format\n");
      type = branches;
    }
    else if (opcode == OPCODE_BNE) {
      controlUnit.RegDst = 0;  //X in book
      controlUnit.ALUsrc = 0;
      controlUnit.MemToReg = 0;  //X in book
      controlUnit.RegWrite = 0;
      controlUnit.MemRead = 0;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 1;
      controlUnit.ALUop = 0b01;
      controlUnit.Jump = 0;
      //printf("BEQ-format\n");
      type = branches;
    }
    else if (opcode == OPCODE_BLTZ) {
      controlUnit.RegDst = 0;  //X in book
      controlUnit.ALUsrc = 0;
      controlUnit.MemToReg = 0;  //X in book
      controlUnit.RegWrite = 0;
      controlUnit.MemRead = 0;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 1;
      controlUnit.ALUop = 0b01;
      controlUnit.Jump = 0;
      //printf("BEQ-format\n");

      type = branches;
    }

    //Jump-formats (ADD TO ALU)
    else if (opcode == OPCODE_J || opcode == OPCODE_JAL) {
      controlUnit.RegDst = 0;  //X in book
      controlUnit.ALUsrc = 0;
      controlUnit.MemToReg = 0;  //X in book
      controlUnit.RegWrite = 0;
      controlUnit.MemRead = 0;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 0;
      controlUnit.ALUop = -1;
      controlUnit.Jump = 1;
      printf("Jump Type\n");
      type = jumps;
    }

    else if (opcode == OPCODE_JAL) {
      controlUnit.RegDst = 0;  //X in book
      controlUnit.ALUsrc = 0;
      controlUnit.MemToReg = 0;  //X in book
      controlUnit.RegWrite = 0;
      controlUnit.MemRead = 0;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 0;
      controlUnit.ALUop = 0;
      controlUnit.Jump = 1;
      printf("Jump Type\n");
      type = jumps;
    }


    //I-formats (i.e. the remaining ones)
    else if (opcode == OPCODE_ADDI) {

      controlUnit.RegDst = 0;  //X in book
      controlUnit.ALUsrc = 1;
      controlUnit.MemToReg = 0;  //X in book
      controlUnit.RegWrite = 1;
      controlUnit.MemRead = 0;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 0;
      controlUnit.ALUop = -1;
      controlUnit.Jump = 0;
      printf("I-format instructions\n");
      type = Iform;
    }

    else if (opcode == OPCODE_ADDUI) {

      controlUnit.RegDst = 0;  //X in book
      controlUnit.ALUsrc = 1;
      controlUnit.MemToReg = 0;  //X in book
      controlUnit.RegWrite = 1;
      controlUnit.MemRead = 0;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 0;
      controlUnit.ALUop = -1;
      controlUnit.Jump = 0;
      printf("I-format instructions\n");
      type = Iform;
    }

    else if (opcode == OPCODE_ANDI) {

      controlUnit.RegDst = 0;  //X in book
      controlUnit.ALUsrc = 1;
      controlUnit.MemToReg = 0;  //X in book
      controlUnit.RegWrite = 1;
      controlUnit.MemRead = 0;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 0;
      controlUnit.ALUop = -1;
      controlUnit.Jump = 0;
      printf("I-format instructions\n");
      type = Iform;
    }
    else if (opcode == OPCODE_XORI) {

      controlUnit.RegDst = 0;  //X in book
      controlUnit.ALUsrc = 1;
      controlUnit.MemToReg = 0;  //X in book
      controlUnit.RegWrite = 1;
      controlUnit.MemRead = 0;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 0;
      controlUnit.ALUop = -1;
      controlUnit.Jump = 0;
      printf("I-format instructions\n");
      type = Iform;
    }

    else if (opcode == OPCODE_ORI) {
        printf("\n \n \n ORI \n");
      controlUnit.RegDst = 0;  //X in book
      controlUnit.ALUsrc = 1;
      controlUnit.MemToReg = 0;  //X in book
      controlUnit.RegWrite = 1;
      controlUnit.MemRead = 0;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 0;
      controlUnit.ALUop = -1;
      controlUnit.Jump = 0;
      printf("I-format instructions\n");
      type = Iform;
    }

    else if (opcode == OPCODE_SLTI) {

      controlUnit.RegDst = 0;  //X in book
      controlUnit.ALUsrc = 1;
      controlUnit.MemToReg = 0;  //X in book
      controlUnit.RegWrite = 1;
      controlUnit.MemRead = 0;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 0;
      controlUnit.ALUop = -1;
      controlUnit.Jump = 0;
      printf("I-format instructions\n");
      type = Iform;
    }

    else if (opcode == OPCODE_SLTUI) {

      controlUnit.RegDst = 0;  //X in book
      controlUnit.ALUsrc = 1;
      controlUnit.MemToReg = 0;  //X in book
      controlUnit.RegWrite = 1;
      controlUnit.MemRead = 0;
      controlUnit.MemWrite = 0;
      controlUnit.Branch = 0;
      controlUnit.ALUop = -1;
      controlUnit.Jump = 0;
      printf("I-format instructions\n");
      type = Iform;
    }

    else {
        printf("WARNING: Control lines not set.\n");
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
