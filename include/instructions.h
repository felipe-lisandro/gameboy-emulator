#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "cpu.h"

#define X(op, name, cycles, func, args) void func(CPU *cpu, void *args);
#include "instructions.def"
#undef X

typedef struct Instruction {
    char *mnemonic;
    void (*execute)(CPU* cpu, void *args);
    void *args;
    int cycles;
} Instruction;

extern Instruction InstructionTable[256];

#endif 