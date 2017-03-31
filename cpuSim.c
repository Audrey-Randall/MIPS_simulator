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


int32_t ALU(uint8_t input1, uint8_t input2, uint8_t err, uint8_t result) {
  //return x ALUop y
  uint8_t oper = 0x0;
  err = 0;
  switch(controlUnit.ALUop){
    case 0b00: //this means that it's a load/store instruction
      oper = 0x2; //(0010) - add
      printf("case 1\n");
      break;
    case 0b01: //this means that it's a branch statement
      oper = 0x6; // (0110) - subtract
      printf("case 2\n");
      break;
    case 0b10: //this means that it's an R-type instruction
        printf("case 3\n");
        printf("%d\n",curIns.opcode);
      //analyze funct field
      switch(curIns.opcode) {
          printf("%d\n",curIns.opcode);
        case 0b100000:
            oper = 0x2; //(0010) - add
            break;
        case 0b100010:
            oper = 0x6; //(0110) - subtract
            break;
        case 0b100100: //bit-wise AND
            oper = 0x0; //(0000) - bitwise AND
            break;
        case 0b100101: //bit-wise OR
            oper = 0x1; //(0001) - bitwise OR
            break;
        case 0b101010: //set on less than
            oper = 0x111; //(0111) - set on less than
            break;
        default: err = 1;
      }
      break;
    case 0b11:
        err = 1;
        break;
  }
  if (err = 1) {
    printf("Error with ALU control unit. Please check Opcode and funct field detection.\n");
    return -1;
  }
  switch(oper) {

    //add
    case 0010:
        result = input1 + input2;
        PC+=4;
        break;
    //subtract
    case 0110:
        result = input1 - input2;
        PC+=4;
        break;
    //AND
    case 0000:
        result = input1 & input2;
        PC+=4;
        break;
    //OR
    case 0001:
        result = input1 | input2;
        PC+=4;
        break;
    //set on less than
    case 0111:
        input1 < input2 ? result:result;
        PC+=4;
        break;
  }

  return result;
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
  return ctrl?one:zero; //if ctrl = 1 choose one and if ctril = 0 then choose zero
}

int textFileConversion(FILE *fp, int * instructionMemory) {
    int i,j,num;
    char temp;
    char binstring[32];
    int bin,bin2,bin3;
    char input[255]; //to store each char into an array (to make it a string)
    char * hexarray[8]; //array of strings
    int index = 0;
    int c;
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
        while (status1 || (c = getc(fp)) != ',' && c != EOF);
    }
    fclose(fp);

    //print integer array
    for(i=0; i<index-1; i++) {
        instructionMemory[i] = memory[i]; //move this array into global memory
        //printf("element %d in dec: %d\n",i,instructionMemory[i]);
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
