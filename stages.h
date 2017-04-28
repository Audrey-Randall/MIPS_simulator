#ifndef STAGES_H
#define STAGES_H

#include <stdint.h>

uint32_t ci;
void FetchStage();
//void DecodeStage(uint32_t EOP);
void DecodeStage();
void ExecuteStage();
void MemoryStage();
void WritebackStage();


#endif
