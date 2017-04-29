#ifndef STAGES_H
#define STAGES_H

#include <stdint.h>

uint32_t ci;
void printCurrentInstruction();
void printControls();
int isLoadOrStore(uint16_t oc);
void FetchStage();
void DecodeStage();
void ExecuteStage();
void MemoryStage();
void WritebackStage();


#endif
