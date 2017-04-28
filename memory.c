#include "memory.h"

uint8_t checkValidData(uint32_t tag, int block_addr){
  if (!DCache[block_addr].valid) return 0;
  if (DCache[block_addr].tag != tag) return 0;
  return 1;
}

//We never write from cache to instruc memory
void flushWriteBuffer(){
  memcpy(&(dataMem[writeBuffer.memAddr>>2]), writeBuffer.data, BLOCKSIZE);
}

//TODO: how the hell do we mimic asynchronicity? We need some sort of state machine that will keep track of how much stuff has been updated. Maybe add a "actually written yet" array to Block struct? One bool per word? Then calculate clock cycle penalties artificially?
void loadIntoDCache(uint32_t addr, int block_addr, unsigned int tag, uint32_t start_block_addr){
  //TODO: Set all globals that tell the processor when its data will be available, to calculate cycles, etc
  //Dirty and valid bits set in read/write functions. Tag and data set here.
  DCache[block_addr].tag = tag;
  memcpy(DCache[block_addr].data, &(dataMem[start_block_addr>>2]), BLOCKSIZE);
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
  //printf("Bits for word: %d Block mask: %x Word mask: %x\n", bitsForWord, blockMask, wordMask);
  *tag = (addr >> (32-taglen))&tagMask;
  *block_addr = (addr&blockMask) >> (bitsForWord + bitsForByte);
  *word_addr = (addr&wordMask) >> 2;
  *byte_addr = 0x3 & addr;
  printf("***tag part: 0x%X  block part: 0x%X \n", ((*tag)<<(32-taglen)), (*block_addr)<<(bitsForWord + bitsForByte));
  *start_mem_block = ((*tag)<<(32-taglen)) | ((*block_addr)<<(bitsForWord + bitsForByte)); //word and byte should be 0
  printf("***start_mem_block = 0x%X\n", (*start_mem_block)>>2);
}

//Load instruc from i-cache
void readInstr(){

}

//Store instruc in i-cache
void writeInstr(){

}

//Store data in d-cache
void writeData(uint32_t addr, int32_t data, int* isHit){
  unsigned int tag = 0;
  int block_addr = 0;
  int word_addr = 0;
  int byte_addr = 0;
  uint32_t start_mem_block = 0; //Address in memory corresponding to the start of the block addr is in
  setAddrsData(addr, &tag, &block_addr, &word_addr, &byte_addr, &start_mem_block);

  uint8_t isValid = checkValidData(tag, block_addr);
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
      int shouldFlush; //should flush write buffer?
      //Evict incorrect block
      if(DCache[block_addr].dirty) {
        //Fill write buffer
        memcpy((void*)writeBuffer.data, (void*)DCache[block_addr].data, 16*sizeof(uint32_t));
        writeBuffer.memAddr = start_mem_block;
        //Set bits
        DCache[block_addr].valid = 0;
        DCache[block_addr].dirty = 0;
        shouldFlush = 1;
      } else {
        //Just set bits, data isn't dirty
        DCache[block_addr].valid = 0;
        DCache[block_addr].dirty = 0;
        shouldFlush = 0;
      }
      //Load correct data into cache, write to it, mark it as dirty
      loadIntoDCache(addr, block_addr, tag, start_mem_block);
      DCache[block_addr].dirty = 1;
      DCache[block_addr].data[word_addr] = data;
      if(shouldFlush) flushWriteBuffer();
    } else {
      //Write-through:
      loadIntoDCache(addr, block_addr, tag, start_mem_block);
      DCache[block_addr].dirty = 1;
      DCache[block_addr].data[word_addr] = data;
      //Only need to write the updated word to memory, not the whole block
      dataMem[addr] = data;
    }
  }
}

//Load data from d-cache
void readData(uint32_t addr, int* isHit){
  unsigned int tag = 0;
  int block_addr = 0;
  int word_addr = 0;
  int byte_addr = 0;
  uint32_t start_mem_block = 0; //Address in memory corresponding to the start of the block addr is in
  setAddrsData(addr, &tag, &block_addr, &word_addr, &byte_addr, &start_mem_block);

  uint8_t isValid = checkValidData(tag, block_addr);
  if(isValid) {
    *isHit = 1; //CPU is allowed to just read DCache[block_addr].data[word_addr]
    //TODO: Pass block_addr and word_addr (and byte_addr?) out of this function for the CPU
    //return DCache[block_addr].data[word_addr];
  } else {
    //cache miss
    *isHit = 0;
    if(WRITEBACK) {
      int shouldFlush = 0;
      //Evict incorrect block
      if(DCache[block_addr].dirty) {
        memcpy((void*)writeBuffer.data, (void*)DCache[block_addr].data, 16*sizeof(uint32_t));
        writeBuffer.memAddr = start_mem_block;
        DCache[block_addr].valid = 0;
        DCache[block_addr].dirty = 0;
        shouldFlush = 1;
      } else {
        DCache[block_addr].valid = 0;
        DCache[block_addr].dirty = 0;
      }
      loadIntoDCache(addr, block_addr, tag, start_mem_block);
      if(shouldFlush) flushWriteBuffer();
    } else {
      loadIntoDCache(addr, block_addr, tag, start_mem_block);
    }
    //TODO: The timing will be way off here. Need to figure out how to let the cycles continue, how to break this up?
    //return DCache[block_addr].data[word_addr];
  }
}

void test1(){
  //16 words in memory
  //Block size 4 words
  //2 blocks per cache, 4 blocks in memory

  //KILLER FACT: When you index into memory using addr, you need to shift addr>>2. Addr is byte addressed (but word aligned), memory is word addressed.
  int addr1 = 0xf<<2;
  int addr2 = 0xd<<2;
  int addr3 = 0x7<<2;
  int addr4 = 0x2<<2;
  int isHit = -10;

  unsigned int* tag = malloc(sizeof(int));
  int* block = malloc(sizeof(int));
  int* word = malloc(sizeof(int));
  int* byte = malloc(sizeof(int));

  int i;
  for(i = 0; i < 16; i++){
    dataMem[i] = i;
  }

  //Read on miss: DCache[addr1>>2].data[addr1's block and offset] = dataMem[addr1]
  //byte_addr = 0; word_addr = 3 = 0b11, block = 1, tag = 1
  readData(addr1, &isHit);
  printf("isHit = %d, DCache block 0: \n", isHit);
  for(i = 0; i < BLOCKSIZE; i++) printf("%d, ", DCache[0].data[i]);
  printf("\nDCache block 1:\n");
  for(i = 0; i < BLOCKSIZE; i++) printf("%d, ", DCache[1].data[i]);

  //Write on miss
  printf("\n\n");
  //0xd<<2 = 110100
  //byte_addr = 00, word_addr = 01, block = 1, tag = 1
  writeData(addr2, 256, &isHit);
  printf("isHit = %d, DCache block 0: \n", isHit);
  for(i = 0; i < BLOCKSIZE; i++) printf("%d, ", DCache[0].data[i]);
  printf("\nDCache block 1:\n");
  for(i = 0; i < BLOCKSIZE; i++) printf("%d, ", DCache[1].data[i]);
  printf("\n\n");

}

void test0(){
  //16 words in memory
  //Block size 4 words
  //2 blocks per cache, 4 blocks in memory

  //KILLER FACT: When you index into memory using addr, you need to shift addr>>2. Addr is byte addressed (but word aligned), memory is word addressed.

  //byte_addr = 00; word_addr = 3 = 0b11, block = 1, tag = 1, startmem = 1100
  int addr1 = 0xf<<2;
  //byte_addr = 00, word_addr = 01, block = 1, tag = 1
  int addr2 = 0xd<<2;
  //byte_addr = 00, word = 3, block = 1, tag = 0
  int addr3 = 0x7<<2;
  //byte = 0; word = 2, block = 0, tag = 0
  int addr4 = 0x2<<2;
  int isHit = -10;

  unsigned int* tag = malloc(sizeof(int));
  int* block = malloc(sizeof(int));
  int* word = malloc(sizeof(int));
  int* byte = malloc(sizeof(int));
  uint32_t* start_mem_block = malloc(sizeof(uint32_t));

  int i;
  for(i = 0; i < 16; i++){
    dataMem[i] = i;
  }

  setAddrsData(addr1, tag, block, word, byte, start_mem_block);
  printf("Tag: %x\nBlock: %x\nWord: %x \nByte: %d \nStart_mem_block: %d\n", *tag, *block, *word, *byte, *start_mem_block);
  setAddrsData(addr2, tag, block, word, byte, start_mem_block);
  printf("Tag: %x\nBlock: %x\nWord: %x \nByte: %d \nStart_mem_block: %d\n", *tag, *block, *word, *byte, *start_mem_block);
  setAddrsData(addr3, tag, block, word, byte, start_mem_block);
  printf("Tag: %x\nBlock: %x\nWord: %x \nByte: %d \nStart_mem_block: %d\n", *tag, *block, *word, *byte, *start_mem_block);
  setAddrsData(addr4, tag, block, word, byte, start_mem_block);
  printf("Tag: %x\nBlock: %x\nWord: %x \nByte: %d \nStart_mem_block: %d\n", *tag, *block, *word, *byte, *start_mem_block);
}

int main(int argc, char** argv) {

  memset(writeBuffer.data, 0, BLOCKSIZE); //TODO: transfer this into init
  int i;
  for(i = 0; i < ICACHESIZE/(BLOCKSIZE*4); i++) {
    memset(ICache[i].data, 0, BLOCKSIZE);
  }
  for(i = 0; i < DCACHESIZE/(BLOCKSIZE*4); i++) {
    memset(DCache[i].data, 0, BLOCKSIZE);
  }
  //For 4 blocks, 16 words each. So tag len = 32-8 = 24
  //Tag = 0x81234 block is 1 word is 5 byte is 2
  int addr1 = 0x08123456;
  //tag = 123498 block 1 word 13 byte 2
  int addr2 = 0x12349876;
  //tag = afbf00 block 0 word 4 byte 0
  int addr3 = 0xafbf0010;
  //tag = 208800 block 2 word 7 byte 0
  int addr4 = 0x2088009c;

  //Perform tests
  //test0();
  test1();

  return 0;
}
