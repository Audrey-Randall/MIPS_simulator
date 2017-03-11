#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "cpuSim.h"
#include "control.h"

FILE *file;


int main()
{
    int i,j;
    printf("Starting simulation\n");
    init();
  //printf("Sign ext of 0xffff is %d", signExt(0xffff));
    char hexarray[11];
    char info;
    uint8_t counter = 0;
	file = fopen("Simulation_example.txt","r"); //r is read-only option
	if (file) {
        int chr = 0;
		while(1) {
			if( feof(file) ) break;
			info = fgetc(file);
//			for (i=0; i<11; i++) {
//
//
//                if (i > 1 && info != '\n') {
//                    hexarray[i-2] = info; //sets the first number into char array of hex numbers
//                    //memory[counter] = info;
//                    printf("%c",hexarray[i-2]);
//                }
//			}
//			printf("\n");
			//to print char array of hex numbers
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
