#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "cpu.h"

#define X(op, name, cycles, func) void func();
#include "instructions.def"
#undef X

typedef struct Instruction {
    char *mnemonic;
    void (*execute)(CPU* cpu);
    int cycles;
} Instruction;

extern Instruction InstructionTable[256];

#endif 