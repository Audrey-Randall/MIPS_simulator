#ifndef MEMORY_H
#define MEMORY_H

#define DEBUG_CACHE

#include <math.h>
#ifdef DEBUG_CACHE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#endif

#define MEMSIZE 1200

//Three block sizes: 1, 4, 16. I'm assuming this is in words.
#define BLOCKSIZE 16

//Use writeback or write-through
//On write-through, any instruction that writes to the cache writes to memory at the same time
#define WRITEBACK 0

//Cache sizes in bytes
#define ICACHESIZE 128
#define DCACHESIZE 256

int32_t writeBuffer[BLOCKSIZE]; //TODO: needs to be 1 word for write-back

//Data is stored as 32-bit integers. If you need to address in to a byte, you need a mask.
typedef struct Block {
  int32_t data[BLOCKSIZE];
  uint8_t valid:1;
  uint8_t dirty:1;
  uint32_t tag;
}Block;

Block ICache[ICACHESIZE/BLOCKSIZE];
Block DCache[DCACHESIZE/BLOCKSIZE];

uint32_t memory[MEMSIZE];    //Program memory
int32_t dataMem[MEMSIZE];    //Data memory. TODO: Should this be signed or unsigned or does it matter?
#endif
