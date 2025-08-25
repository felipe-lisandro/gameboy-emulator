#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#define X(op, name, cycles, func) void func();
#include "instructions.def"
#undef X

typedef struct Instruction {
    char *mnemonic;
    void (*execute)(void);
    int cycles;
} Instruction;

extern Instruction InstructionTable[256];

#endif 