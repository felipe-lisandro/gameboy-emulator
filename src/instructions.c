#include "instructions.h"
#include "helper.h"

#define X(op, name, cycles, func, args)[op] = {name, cycles, func, args},
Instruction instruction_table[256] = {
    #include "instructions.def"
};
#undef X

void instr_NOP(CPU *cpu){

}

void instr_INC_8(CPU *cpu, void *args){
    uint8_t *reg = (uint8_t *) args;
    uint8_t old = *reg;
    (*reg)++;
    // zero flag
    if (*reg == 0) SET_FLAG(cpu, FLAG_Z);
    else CLEAR_FLAG(cpu, FLAG_Z);
    // clear subtract
    CLEAR_FLAG(cpu, FLAG_N);
    // if there's half carry, store
    if ((old & 0x0F) + 1 > 0x0F) SET_FLAG(cpu, FLAG_H);
    else CLEAR_FLAG(cpu, FLAG_H);
}

void instr_DEC_8(CPU *cpu, void *args){
    uint8_t *reg = (uint8_t *) args;
    uint8_t old = *reg;
    (*reg)--;
    // zero flag
    if (*reg == 0) SET_FLAG(cpu, FLAG_Z);
    else CLEAR_FLAG(cpu, FLAG_Z);
    // set subtract
    SET_FLAG(cpu, FLAG_N);
    // if there's half carry, store
    if ((old & 0x0F) + 1 > 0x0F) SET_FLAG(cpu, FLAG_H);
    else CLEAR_FLAG(cpu, FLAG_H);
}
