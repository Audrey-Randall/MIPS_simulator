#ifndef MEMORY_H
#define MEMORY_H

//#define MEMSIZE 4096
#define MEMSIZE 1200

//Three block sizes: 1, 4, 16
#define BLOCKSIZE 16

//Use writeback or write-through
//On write-through, any instruction that writes to the cache writes to memory at the same time
#define WRITEBACK 0

//Cache sizes
#define ICACHESIZE 128
#define DCACHESIZE 256

//TODO: declare write buffer. Block sized?

uint32_t memory[MEMSIZE];    //Program memory
int32_t dataMem[MEMSIZE];    //Data memory. TODO: Should this be signed or unsigned or does it matter? signed. as datamem can be neg
#endif
