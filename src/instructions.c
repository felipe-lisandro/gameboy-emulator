#include "instructions.h"

#define X(op, name, cycles, func) [op] = { name, cycles, func },
Instruction instruction_table[256] = {
    #include "instructions.def"
};
#undef X

