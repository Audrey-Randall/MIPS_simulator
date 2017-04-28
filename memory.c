#include "memory.h"

uint8_t isValid(uint16_t tag){
  return 0;
}


void setAddrsData(int32_t addr, unsigned int* tag, int* block_addr, int* word_addr, int* byte_addr, uint32_t* start_mem_block) {
  //int taglen = 32-(log(DCACHESIZE/BLOCKSIZE)/log(2) + log(BLOCKSIZE)/log(2) + 2);
  //*tag = addr >> taglen; //if addr is never negative should always fill with zeroes
  *tag = 0;
  *block_addr = 0;
  *word_addr = 0;
  *byte_addr = 0;
  *start_mem_block = 0;

  int numBlocks = DCACHESIZE/(BLOCKSIZE*4);
  int wordsInBlock = BLOCKSIZE;
  int bitsForBlock = log(numBlocks)/log(2);
  int bitsForWord = log(wordsInBlock)/log(2);
  int bitsForByte = 2;
  int taglen = 32 - (bitsForBlock + bitsForWord + bitsForByte);
  unsigned int blockMask = 0;
  unsigned int wordMask = 0;
  unsigned int tagMask = 0;
  int i;
  //0b11110000 mask from 2^8 to 2^5
  for(i = bitsForWord+bitsForByte; i < 32-taglen; i++) {
    blockMask += 0b1 << i;
  }
  for(i = bitsForByte; i < 32-(taglen+bitsForBlock); i++) {
    wordMask += 0b1 << i;
  }
  for(i = 0; i < taglen; i++) {
    tagMask += 0b1 << i;
  }
  printf("\tBits for word: %d Block mask: %x Word mask: %x\n", bitsForWord, blockMask, wordMask);
  *tag = (addr >> (32-taglen))&tagMask;
  *block_addr = (addr&blockMask) >> (bitsForWord + bitsForByte);
  *word_addr = (addr&wordMask) >> 2;
  *byte_addr = 0x3 & addr;
  *start_mem_block = ((*tag)<<(32-taglen)) & (block_addr)<<(bitsForWord + bitsForByte); //word and byte should be 0
}

//Load instruc from i-cache
void readInstr(){

}

//Store instruc in i-cache
void writeInstr(){

}

//Store data in d-cache
void writeData(uint32_t addr, int32_t data){
  unsigned int tag = 0;
  int block_addr = 0;
  int word_addr = 0;
  int byte_addr = 0;
  uint32_t start_mem_block = 0; //Address in memory corresponding to the start of the block addr is in
  setAddrsData(addr, &tag, &block_addr, &word_addr, &byte_addr, &start_mem_block);

  uint8_t isValid = checkValid(tag, block_addr);
  if(isValid) {
    *isHit = 1;
    DCache[block_addr].dirty = 1;
    DCache[block_addr].valid = 1; //is this redundant?
    DCache[block_addr].data[word_addr] = data;
    //No need to change tag
  } else {
    //cache miss
    *isHit = 0;
    if(WRITEBACK) {
      //Evict incorrect block
      if(DCache[block_addr].dirty) {
        memcpy((void*)writeBuffer, (void*)DCache[block_addr].data, 16*sizeof(uint32_t));
        DCache.valid = 0;
        DCache.dirty = 0;
      } else {
        DCache.valid = 0;
        DCache.dirty = 0;
      }
      //Load correct data into cache, write to it, mark it as dirty
      loadIntoDCache(addr, block_addr, tag, start_mem_block);
      DCache[block_addr].dirty = 1;
      DCache[block_addr].data[word_addr] = data;
      flushWriteBuffer();
    } else {
      //Write-through:
      loadIntoDCache(addr, block_addr, tag, start_mem_block);
      DCache[block_addr].dirty = 1;
      DCache[block_addr].data[word_addr] = data;
      //Only need to write the updated word to memory, not the whole block
      datamem[addr] = data;
    }
}

//Load data from d-cache
void readData(uint32_t addr, uint8_t* isHit){
  unsigned int tag = 0;
  int block_addr = 0;
  int word_addr = 0;
  int byte_addr = 0;
  uint32_t start_mem_block = 0; //Address in memory corresponding to the start of the block addr is in
  setAddrsData(addr, &tag, &block_addr, &word_addr, &byte_addr, &start_mem_block);

  uint8_t isValid = checkValid(tag, block_addr);
  if(isValid) {
    *isHit = 1; //CPU is allowed to just read DCache[block_addr].data[word_addr]
    //TODO: Pass block_addr and word_addr (and byte_addr?) out of this function for the CPU
    //return DCache[block_addr].data[word_addr];
  } else {
    //cache miss
    *isHit = 0;
    if(WRITEBACK) {
      //Evict incorrect block
      if(DCache[block_addr].dirty) {
        memcpy((void*)writeBuffer, (void*)DCache[block_addr].data, 16*sizeof(uint32_t));
        DCache.valid = 0;
        DCache.dirty = 0;
      } else {
        DCache.valid = 0;
        DCache.dirty = 0;
      }
      loadIntoDCache(addr, block_addr, tag, start_mem_block);
    } else {
      loadIntoDCache(addr, block_addr, tag, start_mem_block);
    }
    //TODO: The timing will be way off here. Need to figure out how to let the cycles continue, how to break this up?
    //return DCache[block_addr].data[word_addr];
  }
}

//TODO: how the hell do we mimic asynchronicity? We need some sort of state machine that will keep track of how much stuff has been updated. Maybe add a "actually written yet" array to Block struct? One bool per word? Then calculate clock cycle penalties artificially?
void loadIntoDCache(uint32_t addr, int block_addr, unsigned int tag, uint32_t start_block_addr){
  //TODO: Set all globals that tell the processor when its data will be available, to calculate cycles, etc
  //Dirty and valid bits set in read/write functions. Tag and data set here.
  DCache[block_addr].tag = tag;
  memcpy(DCache[block_addr].data, datamem[start_block_addr], BLOCKSIZE);
}

/*int main(int argc, char** argv) {

  memset(writeBuffer, 0, sizeof(writeBuffer)); //TODO: transfer this into init

  //For 4 blocks, 16 words each. So tag len = 32-8 = 24
  //Tag = 0x81234 block is 1 word is 5 byte is 2
  int addr1 = 0x08123456;
  //tag = 123498 block 1 word 13 byte 2
  int addr2 = 0x12349876;
  //tag = afbf00 block 0 word 4 byte 0
  int addr3 = 0xafbf0010;
  //tag = 208800 block 2 word 7 byte 0
  int addr4 = 0x2088009c;

  unsigned int* tag = malloc(sizeof(unsigned int));
  int* block = malloc(sizeof(int));
  int* word = malloc(sizeof(int));
  int* byte = malloc(sizeof(int));
  setAddrsData(addr1, tag, block, word, byte);
  printf("Tag: %x\nBlock: %x\nWord: %x \nByte: %d \n", *tag, *block, *word, *byte);
  setAddrsData(addr2, tag, block, word, byte);
  printf("Tag: %x\nBlock: %x\nWord: %x \nByte: %d \n", *tag, *block, *word, *byte);
  setAddrsData(addr3, tag, block, word, byte);
  printf("Tag: %x\nBlock: %x\nWord: %x \nByte: %d \n", *tag, *block, *word, *byte);
  setAddrsData(addr4, tag, block, word, byte);
  printf("Tag: %x\nBlock: %x\nWord: %x \nByte: %d \n", *tag, *block, *word, *byte);
}*/
