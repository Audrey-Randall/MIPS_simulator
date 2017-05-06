#ifndef STAGES_H
#define STAGES_H
#include "registers.h"
#include <stdint.h>

uint32_t ins1;
uint32_t ins2;
uint32_t ins3;
uint32_t ins4;
uint32_t ins5;
void printCurrentInstruction();
void printControls();
int isLoadOrStore(uint16_t oc);
void FetchStage(Ifid  IFIDShadow, uint32_t instruc);
void DecodeStage(Ifid IFIDShadow, uint32_t instruc);
void ExecuteStage(Ifid IFIDShadow, uint32_t instruc);
void MemoryStage(Ifid IFIDShadow, uint32_t instruc);
void WritebackStage(Ifid IFIDShadow, uint32_t instruc);


#endif
