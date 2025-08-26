#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "cpu.h"

typedef enum { REG_NONE = 0, REG_A = 1, REG_B = 2, REG_C = 3, REG_D = 4, REG_E = 5, REG_H = 6, REG_L = 7} Reg8;

#define X(op, name, cycles, func, args) void func(CPU *cpu, int args);
#include "instructions.def"
#undef X

typedef struct Instruction {
    char *mnemonic;
    int cycles;
    void (*execute)(CPU* cpu, int args);
    int args;
} Instruction;

extern Instruction InstructionTable[256];

#endif 