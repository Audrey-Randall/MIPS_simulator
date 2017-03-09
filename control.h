#ifndef CONTROL
#define CONTROL

typedef struct ControlUnit {
  uint8_t ALUop:4;
  uint8_t ALUsrc:1;
  uint8_t RegWrite:1;
  uint8_t MemWrite:1;
  uint8_t MemToReg:1;
  uint8_t MemRead:1;
  uint8_t PCsrc:1;
  uint8_t RegDst:1;
}ControlUnit;

ControlUnit controlUnit;
int setControls(); 

#endif
