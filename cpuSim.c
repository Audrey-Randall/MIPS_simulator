#include "cpuSim.h"

//Initialize structs
void init(){
  memset(&controlUnit, 0, sizeof(controlUnit));
  memset(&IFID, 0, sizeof(IFID));
  memset(&IDEX, 0, sizeof(IDEX));
  memset(&EXMEM, 0, sizeof(EXMEM));
  memset(&MEMWB, 0, sizeof(MEMWB));
  memset(&curIns, 0, sizeof(curIns));
  PC = 0;
}

int setCurIns(){
  uint32_t ci = memory[PC];  //current instruction
  uint8_t opcode = (ci & 0xfc000000)>>26; //most significant 6 bits
  if(opcode==0x0){
    //printf("R instruction\n");
    curIns.rs = (ci & 0x03E00000)>>21;
    curIns.rt = (ci & 0x001F0000)>>16;
    curIns.rd = (ci & 0x0000F800)>>11;
    curIns.shamt = (ci & 0x000007C0)>>6;
    curIns.funct = (ci & 0x0000003F);
  } else if (opcode==0x2 || opcode==0x3){
    //printf("J instruc\n");
    curIns.addr = (ci & 0x03FFFFFF);
  } else {
    //printf("I instruc\n");
    curIns.rs = (ci & 0x03E00000)>>21;
    curIns.rt = (ci & 0x001F0000)>>16;
    curIns.imm = (ci & 0x0000ffff);
  }
  #if DEBUG==1
  printf("\n***setCurIns***\n");
  if(opcode==0x0) printf("R function\nRS: %d\nRT: %d\nRD: %d\nShamt: %d\nFunct: %d\n", curIns.rs, curIns.rt, curIns.rd, curIns.shamt, curIns.funct);
  else if(opcode==0x2||opcode==0x3) printf("J instruction\nAddress w/o shift: %04X", curIns.addr);
  else printf("I instruction\nOpcode: %d\nRS: %d\nRT: %d\nImm: %d\n", opcode, curIns.rs, curIns.rt, curIns.imm);
  #endif
  return 0;
}

int32_t ALU(int x, int y) {
  //return x ALUop y
  uint8_t oper = 0x0;
  switch(controlUnit.ALUop){
    case 0b00:
      oper = 0x2;
      break;
    case 0b01:
      oper = 0x6;
      break;
    case 0b10:
      //switch (funct field of instruc)
      break;
    case 0b11:
      printf("ERROR: ALUop was 0x11, Audrey didn't think this was possible\n");
  }
  return 0;
}

int32_t signExt(int32_t offsetField) {
  uint8_t sign = (offsetField >> 15) & 0x1;
  uint32_t mask = sign?0xffff0000:0x00000000;
  return (int32_t)(offsetField|mask);
}

int dataMemoryUnit(int32_t addr, int32_t writeData){
  return 0;
}

int32_t mux(int32_t zero, int32_t one, uint8_t ctrl){
  return ctrl?one:zero;
}

void FetchStage(){
  setCurIns();
}
void DecodeStage(){
  setControls(curIns.opcode);
  //Read 2 registers to pass to ALU or read one register and use signExt unit for immediate value
  int arg1, arg2;
  arg1 = regs[curIns.rs];
  arg2 = controlUnit.ALUsrc ? signExt(curIns.imm) : regs[curIns.rt];
  ALU(arg1, arg2);
}
void ExecuteStage(){

}
void MemoryStage(){

}
void WritebackStage(){

}

int textFileConversion(FILE *fp, int * instructionMemory) {
    int i,j,num;
    char temp;
    int numex = 1;
    //long binary;
    char binstring[32];
    int bin,bin2,bin3;
    char input[255]; //to store each char into an array (to make it a string)
    char * hexarray[8]; //array of strings
    int index = 0;
    int status1,number;
    fp = fopen("Simulation_example.txt","r");
    if (fp == NULL) {
        printf("cannot open file.\n");
        return -1;
    }

    //store each character into a string, print each line
    else {
        do
        {
            status1 = fgets(input, sizeof(input), fp);
            number = (int)strtol(input, NULL, 0); //converts hex string number to int
            hexarray[index] = input;
            //printf("%s\n",hexarray[index]); //to store hex addresses before converting them
            memory[index] = number;
            index++; //counts how many instructions are in text file.
        }
        while (status1);
    }
    fclose(fp);

    //print integer array
    for(i=0; i<index-1; i++) {
        instructionMemory[i] = memory[i]; //move this array into global memory
        printf("memory in dec: %d\n",instructionMemory[i]);
    }


    //convert integers to binary numbers
    /*for (j=0; j<index-1; j++) {

        printf("Binary versions from memory\n");
        //actual versions needed:
        bin = convert(instructionMemory[j]);
        bin2 = int_to_int(instructionMemory[j]);
        bin3 = decimalToBinary(instructionMemory[j]);
        printf("num1: %d, num2: %d, num3: %d\n",bin,bin2,bin3);

        //trial just to test
        printf("binary version of small decimal numbers:\n");
        bin = convert(11);
        bin2 = int_to_int(11);
        bin3 = decimalToBinary(11);
        printf("num1: %d, num2: %d, num3: %d\n",bin,bin2,bin3);

        //function output
        instructionMemory[j] = bin2; //or bin or bin3
    }*/

    return 0;
}

/*int int_to_int(unsigned int k) {
    char buffer[128]; // any number higher than sizeof(unsigned int)*bits_per_byte(8)
    return atoi( itoa(k, buffer, 2) );
}

void getBin(int num, char *str)
{
  *(str+32) = '\0'; //make sure that the string ends in a null char
  int mask = 0x100 << 1; //created a mask shifted to the left once to account for first shift
  while(mask >>= 1) {
    *str++ = !!(mask & num) + '0';
  }
}

int convert(int dec) {
    if (dec == 0) {
        return;
    }
    else return (dec % 2 + 10*convert(dec / 2));
}

int decimalToBinary(int n) {
    int remainder;
    int binary = 0, i = 1;

    while(n != 0) {
        remainder = n%2;
        n = n/2;
        binary= binary + (remainder*i);
        i = i*10;
        //printf("%d", binary);
    }
    return binary;
}*/
