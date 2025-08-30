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

uint16_t* get_16bit_reg(CPU* cpu, Reg16 r) {
    switch(r){
        case REG_NONE: return 0;
        case REG_BC: return get_BC(cpu);
        case REG_DE: return get_DE(cpu);
        case REG_HL: return get_HL(cpu);
        case REG_SP: return get_SP(cpu);
        case REG_AF: return get_AF(cpu);
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

void instr_INC_16(CPU *cpu, int args){
    Reg16 r = args;
    uint16_t val = 0;
    switch(r){
        case REG_HL: val = get_HL(cpu) + 1; set_HL(cpu, val); break;
        case REG_BC: val = get_BC(cpu) + 1; set_BC(cpu, val); break;
        case REG_DE: val = get_DE(cpu) + 1; set_DE(cpu, val); break;
        case REG_SP: cpu->SP++; break;
        default: return;
    }
}

void instr_INC_HL(CPU *cpu, int args){
    uint16_t reg = get_HL(cpu);
    uint8_t value = mem_read(reg);
    value++;
    mem_write(reg, value);
    if(value == 0) SET_FLAG(cpu, FLAG_Z); else CLEAR_FLAG(cpu, FLAG_Z);
    if((value & 0x0F) == 0x00) SET_FLAG(cpu, FLAG_H); else CLEAR_FLAG(cpu, FLAG_H);
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

void instr_ADD_A_8(CPU* cpu, int args){
    uint8_t reg = get_8bit_reg(cpu, (Reg8)args);
    uint16_t result = cpu->A + reg;
    if((result & 0xFF) == 0) SET_FLAG(cpu, FLAG_Z); else CLEAR_FLAG(cpu, FLAG_Z);
    CLEAR_FLAG(cpu, FLAG_N);
    if(((cpu->A & 0xF) + (reg & 0xF)) & 0x10) SET_FLAG(cpu, FLAG_H); else CLEAR_FLAG(cpu, FLAG_H);
    if(result > 0xFF) SET_FLAG(cpu, FLAG_C); else CLEAR_FLAG(cpu, FLAG_C);
    cpu->A = result & 0xFF;
}

void instr_ADC_A_8(CPU* cpu, int args){
    uint8_t reg = get_8bit_reg(cpu, (Reg8)args);
    uint16_t result = cpu->A + reg + GET_FLAG(cpu, FLAG_C);
    if((result & 0xFF) == 0) SET_FLAG(cpu, FLAG_Z); else CLEAR_FLAG(cpu, FLAG_Z);
    CLEAR_FLAG(cpu, FLAG_N);
    if(((cpu->A & 0xF) + (reg & 0xF)) & 0x10) SET_FLAG(cpu, FLAG_H); else CLEAR_FLAG(cpu, FLAG_H);
    if(result > 0xFF) SET_FLAG(cpu, FLAG_C); else CLEAR_FLAG(cpu, FLAG_C);
    cpu->A = result & 0xFF;
}

void instr_ADC_A_HL(CPU* cpu, int args){
    uint8_t reg = get_8bit_reg(cpu, (Reg8)args);
    uint16_t result = cpu->A + reg + GET_FLAG(cpu, FLAG_C);
    if((result & 0xFF) == 0) SET_FLAG(cpu, FLAG_Z); else CLEAR_FLAG(cpu, FLAG_Z);
    CLEAR_FLAG(cpu, FLAG_N);
    if(((cpu->A & 0xF) + (reg & 0xF)) & 0x10) SET_FLAG(cpu, FLAG_H); else CLEAR_FLAG(cpu, FLAG_H);
    if(result > 0xFF) SET_FLAG(cpu, FLAG_C); else CLEAR_FLAG(cpu, FLAG_C);
    cpu->A = result & 0xFF;
}

void instr_ADC_A_D8(CPU* cpu, int args){
    uint8_t val = mem_read(cpu->PC++);
    uint16_t result = cpu->A + val + GET_FLAG(cpu, FLAG_C);
    if((result & 0xFF) == 0) SET_FLAG(cpu, FLAG_Z); else CLEAR_FLAG(cpu, FLAG_Z);
    CLEAR_FLAG(cpu, FLAG_N);
    if(((cpu->A & 0xF) + (val & 0xF)) & 0x10) SET_FLAG(cpu, FLAG_H); else CLEAR_FLAG(cpu, FLAG_H);
    if(result > 0xFF) SET_FLAG(cpu, FLAG_C); else CLEAR_FLAG(cpu, FLAG_C);
    cpu->A = result & 0xFF;
}

void instr_SUB_A_8(CPU* cpu, int args){
    uint8_t reg = get_8bit_reg(cpu, (Reg8)args);
    uint16_t result = cpu->A - reg;
    if((result & 0xFF) == 0) SET_FLAG(cpu, FLAG_Z); else CLEAR_FLAG(cpu, FLAG_Z);
    SET_FLAG(cpu, FLAG_N);
    if((cpu->A & 0xF) + (reg & 0xF)) SET_FLAG(cpu, FLAG_H); else CLEAR_FLAG(cpu, FLAG_H);
    if(result < reg) SET_FLAG(cpu, FLAG_C); else CLEAR_FLAG(cpu, FLAG_C);
    cpu->A = result & 0xFF;
}

void instr_SUB_D8(CPU *cpu, int args){
    uint8_t value = mem_read(cpu->PC++);
    uint16_t result = cpu->A - value;
    SET_FLAG(cpu, FLAG_N);
    if ((result & 0x10000) != 0) SET_FLAG(cpu, FLAG_C); else CLEAR_FLAG(cpu, FLAG_C);
    if (((value & 0x0FFF) + (cpu->A & 0x0FFF)) & 0x1000) SET_FLAG(cpu, FLAG_H); else CLEAR_FLAG(cpu, FLAG_H);
    cpu->A = result & 0xFF;
}

void instr_SBC_A_8(CPU* cpu, int args){
    uint8_t reg = get_8bit_reg(cpu, (Reg8)args);
    uint16_t result = cpu->A - reg - GET_FLAG(cpu, FLAG_C);
    if((result & 0xFF) == 0) SET_FLAG(cpu, FLAG_Z); else CLEAR_FLAG(cpu, FLAG_Z);
    SET_FLAG(cpu, FLAG_N);
    if((cpu->A & 0xF) + (reg & 0xF)) SET_FLAG(cpu, FLAG_H); else CLEAR_FLAG(cpu, FLAG_H);
    if(result < reg) SET_FLAG(cpu, FLAG_C); else CLEAR_FLAG(cpu, FLAG_C);
    cpu->A = result & 0xFF;
}

void instr_SBC_HL(CPU* cpu, int args){
    uint8_t reg = mem_read(get_HL(cpu));
    uint16_t result = cpu->A - reg - GET_FLAG(cpu, FLAG_C);
    if((result & 0xFF) == 0) SET_FLAG(cpu, FLAG_Z); else CLEAR_FLAG(cpu, FLAG_Z);
    SET_FLAG(cpu, FLAG_N);
    if((cpu->A & 0xF) + (reg & 0xF)) SET_FLAG(cpu, FLAG_H); else CLEAR_FLAG(cpu, FLAG_H);
    if(result < reg) SET_FLAG(cpu, FLAG_C); else CLEAR_FLAG(cpu, FLAG_C);
    cpu->A = result & 0xFF;
}

void instr_SBC_A_D8(CPU* cpu, int args){
    uint8_t val = mem_read(cpu->PC++);
    uint16_t result = cpu->A - val - GET_FLAG(cpu, FLAG_C);
    if((result & 0xFF) == 0) SET_FLAG(cpu, FLAG_Z); else CLEAR_FLAG(cpu, FLAG_Z);
    SET_FLAG(cpu, FLAG_N);
    if(((cpu->A & 0xF) + (val & 0xF)) & 0x10) SET_FLAG(cpu, FLAG_H); else CLEAR_FLAG(cpu, FLAG_H);
    if(result > 0xFF) SET_FLAG(cpu, FLAG_C); else CLEAR_FLAG(cpu, FLAG_C);
    cpu->A = result & 0xFF;
}

void instr_SUB_HL(CPU *cpu, int args){
    uint8_t value = mem_read(get_HL(cpu));
    uint16_t result = cpu->A - value;
    SET_FLAG(cpu, FLAG_N);
    if ((result & 0x10000) != 0) SET_FLAG(cpu, FLAG_C); else CLEAR_FLAG(cpu, FLAG_C);
    if (((value & 0x0FFF) + (cpu->A & 0x0FFF)) & 0x1000) SET_FLAG(cpu, FLAG_H); else CLEAR_FLAG(cpu, FLAG_H);
    cpu->A = result & 0xFF;
}

void instr_DEC_16(CPU *cpu, int args){
    Reg16 r = args;
    uint16_t val = 0;
    switch(r){
        case REG_HL: val = get_HL(cpu) - 1; set_HL(cpu, val); break;
        case REG_BC: val = get_BC(cpu) - 1; set_BC(cpu, val); break;
        case REG_DE: val = get_DE(cpu) - 1; set_DE(cpu, val); break;
        case REG_SP: cpu->SP--; break;
        default: return;
    }
}

void instr_DEC_HL(CPU *cpu, int args){
    uint16_t reg = get_HL(cpu);
    uint8_t value = mem_read(reg);
    value--;
    mem_write(reg, value);
    if(value == 0) SET_FLAG(cpu, FLAG_Z); else CLEAR_FLAG(cpu, FLAG_Z);
    if((value & 0x0F) == 0x00) SET_FLAG(cpu, FLAG_H); else CLEAR_FLAG(cpu, FLAG_H);
}

void instr_ADD_HL_16(CPU *cpu, int args){
    uint16_t hl = get_HL(cpu);
    uint16_t operand = 0;
    switch((Reg16)args){
        case REG_HL: operand = hl; break;
        case REG_BC: operand = get_BC(cpu); break;
        case REG_DE: operand = get_DE(cpu); break;
        case REG_SP: operand = get_SP(cpu); break;
        default: return;
    }
    uint32_t result = hl + operand;
    CLEAR_FLAG(cpu, FLAG_N);
    if ((result & 0x10000) != 0) SET_FLAG(cpu, FLAG_C); else CLEAR_FLAG(cpu, FLAG_C);
    if (((hl & 0x0FFF) + (operand & 0x0FFF)) & 0x1000) SET_FLAG(cpu, FLAG_H); else CLEAR_FLAG(cpu, FLAG_H);
    set_HL(cpu, (uint16_t)result);
}

void instr_ADD_A_HL(CPU *cpu, int args){
    uint8_t value = mem_read(get_HL(cpu));
    uint16_t result = cpu->A + value;
    CLEAR_FLAG(cpu, FLAG_N);
    if ((result & 0x10000) != 0) SET_FLAG(cpu, FLAG_C); else CLEAR_FLAG(cpu, FLAG_C);
    if (((value & 0x0FFF) + (cpu->A & 0x0FFF)) & 0x1000) SET_FLAG(cpu, FLAG_H); else CLEAR_FLAG(cpu, FLAG_H);
    cpu->A = result & 0xFF;
}

void instr_ADD_A_D8(CPU *cpu, int args){
    uint8_t value = mem_read(cpu->PC++);
    uint16_t result = cpu->A + value;
    CLEAR_FLAG(cpu, FLAG_N);
    if ((result & 0x10000) != 0) SET_FLAG(cpu, FLAG_C); else CLEAR_FLAG(cpu, FLAG_C);
    if (((value & 0x0FFF) + (cpu->A & 0x0FFF)) & 0x1000) SET_FLAG(cpu, FLAG_H); else CLEAR_FLAG(cpu, FLAG_H);
    cpu->A = result & 0xFF;
}

void instr_AND_8(CPU *cpu, int args){
    uint8_t *reg = get_8bit_reg(cpu, (Reg8)args);
    cpu->A &= *reg;
    if(cpu->A == 0x00) SET_FLAG(cpu, FLAG_Z); else CLEAR_FLAG(cpu, FLAG_Z);
    CLEAR_FLAG(cpu, FLAG_N);
    SET_FLAG(cpu, FLAG_H);
    CLEAR_FLAG(cpu, FLAG_C);
}

void instr_AND_D8(CPU *cpu, int args){
    uint8_t val = mem_read(cpu->PC++);
    cpu->A &= val;
    if(cpu->A == 0x00) SET_FLAG(cpu, FLAG_Z); else CLEAR_FLAG(cpu, FLAG_Z);
    CLEAR_FLAG(cpu, FLAG_N);
    SET_FLAG(cpu, FLAG_H);
    CLEAR_FLAG(cpu, FLAG_C);
}

void instr_AND_HL(CPU *cpu, int args){
    uint16_t val = mem_read(get_HL(cpu));
    cpu->A &= val;
    if(cpu->A == 0x00) SET_FLAG(cpu, FLAG_Z); else CLEAR_FLAG(cpu, FLAG_Z);
    CLEAR_FLAG(cpu, FLAG_N);
    SET_FLAG(cpu, FLAG_H);
    CLEAR_FLAG(cpu, FLAG_C);
}

void instr_OR_8(CPU *cpu, int args){
    uint8_t *reg = get_8bit_reg(cpu, (Reg8)args);
    cpu->A |= *reg;
    if(cpu->A == 0x00) SET_FLAG(cpu, FLAG_Z); else CLEAR_FLAG(cpu, FLAG_Z);
    CLEAR_FLAG(cpu, FLAG_N);
    CLEAR_FLAG(cpu, FLAG_H);
    CLEAR_FLAG(cpu, FLAG_C);
}

void instr_OR_HL(CPU *cpu, int args){
    uint16_t val = mem_read(get_HL(cpu));
    cpu->A |= val;
    if(cpu->A == 0x00) SET_FLAG(cpu, FLAG_Z); else CLEAR_FLAG(cpu, FLAG_Z);
    CLEAR_FLAG(cpu, FLAG_N);
    CLEAR_FLAG(cpu, FLAG_H);
    CLEAR_FLAG(cpu, FLAG_C);
}

void instr_OR_D8(CPU *cpu, int args){
    uint16_t val = mem_read(cpu->PC++);
    cpu->A |= val;
    if(cpu->A == 0x00) SET_FLAG(cpu, FLAG_Z); else CLEAR_FLAG(cpu, FLAG_Z);
    CLEAR_FLAG(cpu, FLAG_N);
    CLEAR_FLAG(cpu, FLAG_H);
    CLEAR_FLAG(cpu, FLAG_C);
}

void instr_XOR_8(CPU *cpu, int args){
    uint8_t *reg = get_8bit_reg(cpu, (Reg8)args);
    cpu->A ^= *reg;
    if(cpu->A == 0x00) SET_FLAG(cpu, FLAG_Z); else CLEAR_FLAG(cpu, FLAG_Z);
    CLEAR_FLAG(cpu, FLAG_N);
    CLEAR_FLAG(cpu, FLAG_H);
    CLEAR_FLAG(cpu, FLAG_C);
}

void instr_XOR_D8(CPU *cpu, int args){
    uint16_t val = mem_read(cpu->PC++);
    cpu->A ^= val;
    if(cpu->A == 0x00) SET_FLAG(cpu, FLAG_Z); else CLEAR_FLAG(cpu, FLAG_Z);
    CLEAR_FLAG(cpu, FLAG_N);
    CLEAR_FLAG(cpu, FLAG_H);
    CLEAR_FLAG(cpu, FLAG_C);
}

void instr_XOR_HL(CPU *cpu, int args){
    uint16_t val = mem_read(get_HL(cpu));
    cpu->A ^= val;
    if(cpu->A == 0x00) SET_FLAG(cpu, FLAG_Z); else CLEAR_FLAG(cpu, FLAG_Z);
    CLEAR_FLAG(cpu, FLAG_N);
    CLEAR_FLAG(cpu, FLAG_H);
    CLEAR_FLAG(cpu, FLAG_C);
}

void instr_CP_8(CPU *cpu, int args){
    uint8_t *reg = get_8bit_reg(cpu, (Reg8)args);
    if((cpu->A - *reg) == 0) SET_FLAG(cpu, FLAG_Z); else CLEAR_FLAG(cpu, FLAG_Z);
    CLEAR_FLAG(cpu, FLAG_N);
    CLEAR_FLAG(cpu, FLAG_H);
    CLEAR_FLAG(cpu, FLAG_C);
}

void instr_CP_HL(CPU *cpu, int args){
    uint8_t reg = mem_read(get_HL(cpu));
    if((cpu->A - reg) == 0) SET_FLAG(cpu, FLAG_Z); else CLEAR_FLAG(cpu, FLAG_Z);
    CLEAR_FLAG(cpu, FLAG_N);
    CLEAR_FLAG(cpu, FLAG_H);
    CLEAR_FLAG(cpu, FLAG_C);
}

void instr_CP_D8(CPU *cpu, int args){
    uint8_t reg = mem_read(cpu->PC++);
    if((cpu->A - reg) == 0) SET_FLAG(cpu, FLAG_Z); else CLEAR_FLAG(cpu, FLAG_Z);
    CLEAR_FLAG(cpu, FLAG_N);
    CLEAR_FLAG(cpu, FLAG_H);
    CLEAR_FLAG(cpu, FLAG_C);
}

void instr_LD_8_U8(CPU *cpu, int args){
    Reg8 r = args;
    uint8_t *reg = get_8bit_reg(cpu, r);
    if(!reg) return;
    uint8_t imm = mem_read(cpu->PC++);
    *reg = imm;
}

void instr_LD_16_U16(CPU *cpu, int args){
    uint16_t value = mem_read(cpu->PC++);
    value |= mem_read(cpu->PC++) << 8;
    switch((Reg16)args){
        case REG_HL: set_HL(cpu, value) ;break;
        case REG_BC: set_BC(cpu, value); break;
        case REG_DE: set_DE(cpu, value); break;
        case REG_SP: set_SP(cpu, value); break;
        default: return;
    }
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

void instr_LD_8_HL(CPU *cpu, int args){
    Reg8 r = args;
    uint8_t *reg = get_8bit_reg(cpu, r);
    uint16_t hl = get_HL(cpu);
    *reg = mem_read(get_HL);
}

void instr_LD_HL_8(CPU *cpu, int args){
    Reg8 r = args;
    uint8_t *reg = get_8bit_reg(cpu, r);
    uint16_t hl = get_HL(cpu);
    mem_write(hl, *reg);
}

void instr_PUSH_16(CPU* cpu, int args){
    printf("push -> %02X\n", cpu->SP);
    uint16_t r;
    switch((Reg16)args){
        case REG_HL: r = get_HL(cpu) ;break;
        case REG_BC: r = get_BC(cpu); break;
        case REG_DE: r = get_DE(cpu); break;
        case REG_SP: r = get_SP(cpu); break;
        case REG_AF: r = get_AF(cpu); break;
        case REG_PC: r = get_PC(cpu); break;
        default: return;
    }
    uint8_t hi = (r >> 8) & 0xFF;
    uint8_t lo = r & 0xFF;
    mem_write(--cpu->SP, lo);
    mem_write(--cpu->SP, hi);
}

void instr_POP_16(CPU* cpu, int args){
    printf("pop -> %02X\n", cpu->SP);
    uint16_t r;
    uint8_t hi = mem_read(cpu->SP++);
    uint8_t lo = mem_read(cpu->SP++);
    uint16_t val = (hi << 8) | lo;
    switch((Reg16)args){
        case REG_HL: set_HL(cpu, val) ;break;
        case REG_BC: set_BC(cpu, val); break;
        case REG_DE: set_DE(cpu, val); break;
        case REG_SP: set_SP(cpu, val); break;
        case REG_AF: set_AF(cpu, val); break;
        case REG_PC: r = get_PC(cpu); break;
        default: return;
    }
}

void instr_JR_s8(CPU *cpu, int args){
    int8_t offset = (int8_t)mem_read(cpu->PC++);
    cpu->PC += offset;
}

void instr_JR_NZ_s8(CPU *cpu, int args){
    int8_t offset = (int8_t)mem_read(cpu->PC++);
    if(!GET_FLAG(cpu, FLAG_Z)){
        cpu->PC += offset;
        cpu->cycles++;
    }
}

void instr_JR_Z_s8(CPU *cpu, int args){
    int8_t offset = (int8_t)mem_read(cpu->PC++);
    if(GET_FLAG(cpu, FLAG_Z)){
        cpu->PC += offset;
        cpu->cycles++;
    }
}

void instr_JR_NC_s8(CPU *cpu, int args){
    int8_t offset = (int8_t)mem_read(cpu->PC++);
    if(!GET_FLAG(cpu, FLAG_C)){
        cpu->PC += offset;
        cpu->cycles++;
    }
}

void instr_JR_C_s8(CPU *cpu, int args){
    int8_t offset = (int8_t)mem_read(cpu->PC++);
    if(GET_FLAG(cpu, FLAG_C)){
        cpu->PC += offset;
        cpu->cycles++;
    }
}

void instr_CPL(CPU *cpu, int args){
    uint8_t *a = get_8bit_reg(cpu, (Reg8)REG_A);
    if(!a) return;
    (*a) = ~(*a);
    SET_FLAG(cpu, FLAG_N);
    SET_FLAG(cpu, FLAG_H);
}

void instr_SCF(CPU *cpu, int args){
    SET_FLAG(cpu, FLAG_C);
    CLEAR_FLAG(cpu, FLAG_N);
    CLEAR_FLAG(cpu, FLAG_H);
}

void instr_CCF(CPU *cpu, int args){
    if(GET_FLAG(cpu, FLAG_C) == 1) CLEAR_FLAG(cpu, FLAG_C);
    else SET_FLAG(cpu, FLAG_C);
    CLEAR_FLAG(cpu, FLAG_N);
    CLEAR_FLAG(cpu, FLAG_H);
}

void instr_RST_N(CPU *cpu, int args){
    cpu->PC++;
    instr_PUSH_16(cpu, REG_PC);
    uint16_t addr = args * 0x08;
    cpu->PC = addr;
}

void instr_RET(CPU *cpu, int args){
    uint8_t hi, lo;
    lo = mem_read(cpu->SP++);
    hi = mem_read(cpu->SP++);
    cpu->PC = ((uint16_t)hi << 8) | lo;
}

void instr_RET_IF(CPU *cpu, int args){
    uint8_t hi, lo;
    lo = mem_read(cpu->SP++);
    hi = mem_read(cpu->SP++);
    int take;
    switch(args) {
        case 1: take = !GET_FLAG(cpu, FLAG_Z); break; // nz
        case 0:  take =  GET_FLAG(cpu, FLAG_Z); break; // z
        case 3: take = !GET_FLAG(cpu, FLAG_C); break; // nc
        case 2:  take =  GET_FLAG(cpu, FLAG_C); break; // c
    }
    if(!take) return;
    cpu->PC = ((uint16_t)hi << 8) | lo;
    cpu->cycles += 3;
}

void instr_JP_A16(CPU *cpu, int args){
    uint8_t hi, lo;
    lo = mem_read(cpu->PC++);
    hi = mem_read(cpu->PC++);
    cpu->PC = ((uint16_t)hi << 8) | lo;
}

void instr_JP_IF_A16(CPU *cpu, int args){
    uint8_t hi, lo;
    lo = mem_read(cpu->PC++);
    hi = mem_read(cpu->PC++);
    int take;
    switch(args) {
        case 1: take = !GET_FLAG(cpu, FLAG_Z); break; // nz
        case 0:  take =  GET_FLAG(cpu, FLAG_Z); break; // z
        case 3: take = !GET_FLAG(cpu, FLAG_C); break; // nc
        case 2:  take =  GET_FLAG(cpu, FLAG_C); break; // c
    }
    if(!take) return;
    cpu->PC = ((uint16_t)hi << 8) | lo;
    cpu->cycles ++;
}

void instr_JP_HL(CPU *cpu, int args){
    uint16_t val = get_16bit_reg(cpu, REG_HL);
    cpu->PC = val;
}

void instr_CALL_A16(CPU *cpu, int args){
    uint8_t hi, lo;
    lo = mem_read(cpu->PC++);
    hi = mem_read(cpu->PC++);
    cpu->PC++;
    instr_PUSH_16(cpu, REG_PC);
    cpu->PC = ((uint16_t)hi << 8) | lo;
}

void instr_CALL_IF_A16(CPU *cpu, int args){
    uint8_t hi, lo;
    lo = mem_read(cpu->PC++);
    hi = mem_read(cpu->PC++);
    int take;
    switch(args) {
        case 1: take = !GET_FLAG(cpu, FLAG_Z); break; // nz
        case 0:  take =  GET_FLAG(cpu, FLAG_Z); break; // z
        case 3: take = !GET_FLAG(cpu, FLAG_C); break; // nc
        case 2:  take =  GET_FLAG(cpu, FLAG_C); break; // c
    }
    if(!take) return;
    cpu->PC++;
    instr_PUSH_16(cpu, REG_PC);
    cpu->PC = ((uint16_t)hi << 8) | lo;
    cpu->cycles += 3;
}