#include "memory.h"

uint8_t isValid(uint16_t tag){
  return 0;
}

//Checked, debugged, working!
void setAddrsData(int32_t addr, unsigned int* tag, int* block_addr, int* word_addr, int* byte_addr) {
  //int taglen = 32-(log(DCACHESIZE/BLOCKSIZE)/log(2) + log(BLOCKSIZE)/log(2) + 2);
  //*tag = addr >> taglen; //if addr is never negative should always fill with zeroes
  *tag = 0;
  *block_addr = 0;
  *word_addr = 0;
  *byte_addr = 0;

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
}

//Load instruc from i-cache
void readInstr(){

}

//Store instruc in i-cache
void writeInstr(){

}

//Store data in d-cache
void writeData(uint32_t addr, int32_t data){
  //log base 2 of number of blocks + log base 2 of blocksize + 2 = bits for tag
  //If we run into FP errors here we may need to round up
  int taglen = 32-(log(DCACHESIZE/BLOCKSIZE)/log(2) + log(BLOCKSIZE)/log(2) + 2);
  int tag = addr >> taglen; //if addr is never negative should always fill with zeroes
  int block_addr = ???
  int word_addr = ???
  uint8_t isValid = ICache[block_addr].valid;
  uint8_t isDirty = ICache[block_addr].dirty;
  if(isValid) {
    *isHit = 1;
    if!(WRITEBACK) {
      //Dirty bit doesn't need to be set for write-through, just write to mem
      datamem[addr] = data;
    } else {
      //writeback
      DCache[block_addr].dirty = 1;
      ???
    }
    DCache[block_addr].data[word_addr] = data;
  } else {
    //cache miss
    //*isHit = 0;
    //loadIntoDCache(addr, block_addr, taglen);
    //return DCache[block_addr].data[word_addr];

  }
}

//Load data from d-cache
int32_t readData(uint32_t addr, uint8_t* isHit){
  //log base 2 of number of blocks + log base 2 of blocksize + 2 = bits for tag
  //If we run into FP errors here we may need to round up

  unsigned int tag = 0;
  int block_addr = 0;
  int word_addr = 0;
  int byte_addr = 0;
  setAddrsData(addr, &tag, &block_addr, &word_addr, &byte_addr);

  uint8_t isValid = ICache[block_addr].valid;
  //uint8_t isDirty = ICache[block_addr].dirty; //don't need this for loads
  if(isValid) {
    *isHit = 1;
    return DCache[block_addr].data[word_addr];
  } else {
    //cache miss
    *isHit = 0;
    loadIntoDCache(addr, block_addr, taglen);
    return DCache[block_addr].data[word_addr];
  }
}

/*void loadIntoDCache(uint32_t addr, int block_addr, int taglen){
  //set valid bit to 1, set tag length, memcpy from datamem into cache[block].data
  DCache[block_addr].valid = 1;
  DCache[block_addr].tag = addr >> taglen;
  int addrStartOfBlock = ???
  memcpy(DCache[block_addr].data, datamem[addrStartOfBlock], BLOCKSIZE);
}*/

int main(int argc, char** argv) {
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
}
