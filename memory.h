#ifndef MEMORY_H
#define MEMORY_H

#define DEBUG_CACHE

#include <math.h>
#include <string.h>
#ifdef DEBUG_CACHE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#endif

/*NOTE: Adresses are byte-aligned. Our memory arrays are word aligned.
*/

//Mem size in words
#define MEMSIZE 16 //1200

//Three block sizes: 1, 4, 16. This is in words.
#define BLOCKSIZE 4

//Use writeback or write-through
//On write-through, any instruction that writes to the cache writes to memory at the same time
#define WRITEBACK 0

//Cache sizes in bytes
#define ICACHESIZE 32 //8 words //128
#define DCACHESIZE 32 //8 words //256

typedef struct WriteBuffer {
  int32_t data[BLOCKSIZE];
  int32_t memAddr; //Address of the start of the block in memory
}WriteBuffer;

//Data is stored as 32-bit integers. If you need to address in to a byte, you need a mask.
typedef struct Block {
  int32_t data[BLOCKSIZE];
  uint8_t valid:1;
  uint8_t dirty:1;
  uint32_t tag;
}Block;

WriteBuffer writeBuffer; //TODO: needs to be 1 word for write-back
Block ICache[ICACHESIZE/(BLOCKSIZE*4)];
Block DCache[DCACHESIZE/(BLOCKSIZE*4)];

uint32_t memory[MEMSIZE];    //Program memory
int32_t dataMem[MEMSIZE];    //Data memory. TODO: Should this be signed or unsigned or does it matter?
#endif
