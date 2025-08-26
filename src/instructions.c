#include "instructions.h"
#include "helper.h"

#define X(op, name, cycles, func) [op] = { name, cycles, func },
Instruction instruction_table[256] = {
    #include "instructions.def"
};
#undef X

void instr_NOP(CPU *cpu){
    
}

// inc's and dec's for each 8=bit register

void instr_INC_A(CPU *cpu){
    inc8(cpu->A);
}

void instr_DEC_A(CPU *cpu){
    dec8(cpu->A);
}

void instr_INC_B(CPU *cpu){
    inc8(cpu->B);
}

void instr_DEC_B(CPU *cpu){
    dec8(cpu->B);
}

void instr_INC_C(CPU *cpu){
    inc8(cpu->C);
}

void instr_DEC_C(CPU *cpu){
    dec8(cpu->C);
}

void instr_INC_D(CPU *cpu){
    inc8(cpu->D);
}

void instr_DEC_D(CPU *cpu){
    dec8(cpu->D);
}

void instr_INC_E(CPU *cpu){
    inc8(cpu->E);
}

void instr_DEC_E(CPU *cpu){
    dec8(cpu->E);
}

void instr_INC_H(CPU *cpu){
    inc8(cpu->H);
}

void instr_DEC_H(CPU *cpu){
    dec8(cpu->H);
}

void instr_INC_L(CPU *cpu){
    inc8(cpu->L);
}

void instr_DEC_L(CPU *cpu){
    dec8(cpu->L);
}