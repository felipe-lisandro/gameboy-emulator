#include "instructions.h"
#include "memory.h"
#include "cpu_instructions.h"
#include <stdio.h>

#define X(op, name, cycles, func, args)[op] = {name, cycles, func, args},
Instruction InstructionTable[256] = {
    #include "instructions.def"
};
#undef X

uint8_t* get_8bit_reg(CPU* cpu, Reg8 r) {
    switch(r){
        case REG_NONE: return 0;
        case REG_A: return &cpu->A;
        case REG_B: return &cpu->B;
        case REG_C: return &cpu->C;
        case REG_D: return &cpu->D;
        case REG_E: return &cpu->E;
        case REG_H: return &cpu->H;
        case REG_L: return &cpu->L;
    }
    return 0;
}

void instr_NOP(CPU *cpu, int args){
    return;
}

void instr_INC_8(CPU *cpu, int args){
    Reg8 r = args;
    uint8_t *reg = get_8bit_reg(cpu, r);
    if(!reg) return;
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

void instr_DEC_8(CPU *cpu, int args){
    Reg8 r = args;
    uint8_t *reg = get_8bit_reg(cpu, r);
    if(!reg) return;
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

void instr_LD_8_U8(CPU *cpu, int args){
    Reg8 r = args;
    uint8_t *reg = get_8bit_reg(cpu, r);
    if(!reg) return;
    uint8_t imm = mem_read(cpu->PC++);
    *reg = imm;
}

void instr_LD_A_8(CPU *cpu, int args){
    Reg8 r = args;
    uint8_t *reg = get_8bit_reg(cpu, r);
    if(!reg) return;
    cpu->A = *reg;
}

void instr_LD_B_8(CPU *cpu, int args){
    Reg8 r = args;
    uint8_t *reg = get_8bit_reg(cpu, r);
    if(!reg) return;
    cpu->B = *reg;
}

void instr_LD_C_8(CPU *cpu, int args){
    Reg8 r = args;
    uint8_t *reg = get_8bit_reg(cpu, r);
    if(!reg) return;
    cpu->C = *reg;
}

void instr_LD_D_8(CPU *cpu, int args){
    Reg8 r = args;    uint8_t *reg = get_8bit_reg(cpu, r);
    if(!reg) return;
    cpu->D = *reg;
}

void instr_LD_E_8(CPU *cpu, int args){
    Reg8 r = args;
    uint8_t *reg = get_8bit_reg(cpu, r);
    if(!reg) return;
    cpu->E = *reg;
}

void instr_LD_H_8(CPU *cpu, int args){
    Reg8 r = args;
    uint8_t *reg = get_8bit_reg(cpu, r);
    if(!reg) return;
    cpu->H = *reg;
}

void instr_LD_L_8(CPU *cpu, int args){
    Reg8 r = args;
    uint8_t *reg = get_8bit_reg(cpu, r);
    if(!reg) return;
    cpu->L = *reg;
}