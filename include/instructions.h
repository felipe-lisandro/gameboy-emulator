#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "cpu.h"

typedef enum { REG_NONE = 0, REG_A = 1, REG_B = 2, REG_C = 3, REG_D = 4, REG_E = 5, REG_H = 6, REG_L = 7} Reg8;
typedef enum { REG_NAN = 0, REG_BC = 1, REG_DE = 2, REG_HL = 3, REG_SP = 4, REG_AF = 5, REG_PC = 6} Reg16;

#define X(op, name, cycles, func, args) void func(CPU *cpu, int args);
#include "instructions8.def"
#include "instructions16.def"
#undef X

typedef struct Instruction {
    char *mnemonic;
    int cycles;
    void (*execute)(CPU* cpu, int args);
    int args;
} Instruction;

extern Instruction InstructionTable[256];
extern Instruction CBInstructionTable[256];

#include <stdint.h>

// Basic instructions
void instr_NOP(CPU *cpu, int args);
void instr_STOP(CPU *cpu, int args);
void instr_HALT(CPU *cpu, int args);

// Increment/Decrement
void instr_INC_8(CPU *cpu, int args);
void instr_INC_16(CPU *cpu, int args);
void instr_INC_HL(CPU *cpu, int args);
void instr_DEC_8(CPU *cpu, int args);
void instr_DEC_16(CPU *cpu, int args);
void instr_DEC_HL(CPU *cpu, int args);

// Arithmetic
void instr_ADD_A_8(CPU* cpu, int args);
void instr_ADD_A_HL(CPU *cpu, int args);
void instr_ADD_A_D8(CPU *cpu, int args);
void instr_ADD_HL_16(CPU *cpu, int args);
void instr_ADD_SP_S8(CPU *cpu, int args);

void instr_ADC_A_8(CPU* cpu, int args);
void instr_ADC_A_HL(CPU* cpu, int args);
void instr_ADC_A_D8(CPU* cpu, int args);

void instr_SUB_A_8(CPU* cpu, int args);
void instr_SUB_HL(CPU *cpu, int args);
void instr_SUB_D8(CPU *cpu, int args);

void instr_SBC_A_8(CPU* cpu, int args);
void instr_SBC_HL(CPU* cpu, int args);
void instr_SBC_A_D8(CPU* cpu, int args);

// Logical
void instr_AND_8(CPU *cpu, int args);
void instr_AND_D8(CPU *cpu, int args);
void instr_AND_HL(CPU *cpu, int args);

void instr_OR_8(CPU *cpu, int args);
void instr_OR_HL(CPU *cpu, int args);
void instr_OR_D8(CPU *cpu, int args);

void instr_XOR_8(CPU *cpu, int args);
void instr_XOR_HL(CPU *cpu, int args);
void instr_XOR_D8(CPU *cpu, int args);

void instr_CP_8(CPU *cpu, int args);
void instr_CP_HL(CPU *cpu, int args);
void instr_CP_D8(CPU *cpu, int args);

// Load instructions
void instr_LD_8_U8(CPU *cpu, int args);
void instr_LD_16_U16(CPU *cpu, int args);
void instr_LD_HL_SP_R8(CPU *cpu, int args);

void instr_LD_A_8(CPU *cpu, int args);
void instr_LD_B_8(CPU *cpu, int args);
void instr_LD_C_8(CPU *cpu, int args);
void instr_LD_D_8(CPU *cpu, int args);
void instr_LD_E_8(CPU *cpu, int args);
void instr_LD_H_8(CPU *cpu, int args);
void instr_LD_L_8(CPU *cpu, int args);

void instr_LD_16_A(CPU *cpu, int args);
void instr_LD_A_16(CPU *cpu, int args);
void instr_LD_A16_SP(CPU *cpu, int args);
void instr_LD_A_ADDR(CPU *cpu, int args);
void instr_LD_ADDR_A(CPU *cpu, int args);
void instr_LD_SP_HL(CPU *cpu, int args);
void instr_LD_8_HL(CPU *cpu, int args);
void instr_LD_HL_8(CPU *cpu, int args);

// Stack instructions
void instr_PUSH_16(CPU* cpu, int args);
void instr_POP_16(CPU* cpu, int args);

// Jump/Call/Return
void instr_JR_s8(CPU *cpu, int args);
void instr_JR_NZ_s8(CPU *cpu, int args);
void instr_JR_Z_s8(CPU *cpu, int args);
void instr_JR_NC_s8(CPU *cpu, int args);
void instr_JR_C_s8(CPU *cpu, int args);

void instr_JP_A16(CPU *cpu, int args);
void instr_JP_IF_A16(CPU *cpu, int args);
void instr_JP_HL(CPU *cpu, int args);

void instr_CALL_A16(CPU *cpu, int args);
void instr_CALL_IF_A16(CPU *cpu, int args);

void instr_RET(CPU *cpu, int args);
void instr_RET_IF(CPU *cpu, int args);
void instr_RETI(CPU *cpu, int args);

// Rotate/Shift
void instr_RLA(CPU* cpu, int args);
void instr_RRA(CPU* cpu, int args);

// Misc
void instr_DAA(CPU *cpu, int args);
void instr_CPL(CPU *cpu, int args);
void instr_SCF(CPU *cpu, int args);
void instr_CCF(CPU *cpu, int args);
void instr_RST_N(CPU *cpu, int args);

// Interrupts
void instr_DI(CPU *cpu, int args);
void instr_EI(CPU *cpu, int args);

void instr_RLC_8(CPU *cpu, int args);
void instr_RLC_HL(CPU *cpu, int args);

void instr_RRC_8(CPU *cpu, int args);
void instr_RRC_HL(CPU *cpu, int args);

void instr_RL_8(CPU *cpu, int args);
void instr_RL_HL(CPU *cpu, int args);

void instr_RR_8(CPU *cpu, int args);
void instr_RR_HL(CPU *cpu, int args);

void instr_SLA_8(CPU *cpu, int args);
void instr_SLA_HL(CPU *cpu, int args);

void instr_SRA_8(CPU *cpu, int args);
void instr_SRA_HL(CPU *cpu, int args);

void instr_SWAP_8(CPU *cpu, int args);
void instr_SWAP_HL(CPU *cpu, int args);

void instr_SRL_8(CPU *cpu, int args);
void instr_SRL_HL(CPU *cpu, int args);

void instr_BIT_0_8(CPU *cpu, int args);
void instr_BIT_0_HL(CPU *cpu, int args);

void instr_BIT_1_8(CPU *cpu, int args);
void instr_BIT_1_HL(CPU *cpu, int args);

void instr_BIT_2_8(CPU *cpu, int args);
void instr_BIT_2_HL(CPU *cpu, int args);

void instr_BIT_3_8(CPU *cpu, int args);
void instr_BIT_3_HL(CPU *cpu, int args);

void instr_BIT_4_8(CPU *cpu, int args);
void instr_BIT_4_HL(CPU *cpu, int args);

void instr_BIT_5_8(CPU *cpu, int args);
void instr_BIT_5_HL(CPU *cpu, int args);

void instr_BIT_6_8(CPU *cpu, int args);
void instr_BIT_6_HL(CPU *cpu, int args);

void instr_BIT_7_8(CPU *cpu, int args);
void instr_BIT_7_HL(CPU *cpu, int args);

void instr_RES_0_8(CPU *cpu, int args);
void instr_RES_0_HL(CPU *cpu, int args);

void instr_RES_1_8(CPU *cpu, int args);
void instr_RES_1_HL(CPU *cpu, int args);

void instr_RES_2_8(CPU *cpu, int args);
void instr_RES_2_HL(CPU *cpu, int args);

void instr_RES_3_8(CPU *cpu, int args);
void instr_RES_3_HL(CPU *cpu, int args);

void instr_RES_4_8(CPU *cpu, int args);
void instr_RES_4_HL(CPU *cpu, int args);

void instr_RES_5_8(CPU *cpu, int args);
void instr_RES_5_HL(CPU *cpu, int args);

void instr_RES_6_8(CPU *cpu, int args);
void instr_RES_6_HL(CPU *cpu, int args);

void instr_RES_7_8(CPU *cpu, int args);
void instr_RES_7_HL(CPU *cpu, int args);

void instr_SET_0_8(CPU *cpu, int args);
void instr_SET_0_HL(CPU *cpu, int args);

void instr_SET_1_8(CPU *cpu, int args);
void instr_SET_1_HL(CPU *cpu, int args);

void instr_SET_2_8(CPU *cpu, int args);
void instr_SET_2_HL(CPU *cpu, int args);

void instr_SET_3_8(CPU *cpu, int args);
void instr_SET_3_HL(CPU *cpu, int args);

void instr_SET_4_8(CPU *cpu, int args);
void instr_SET_4_HL(CPU *cpu, int args);

void instr_SET_5_8(CPU *cpu, int args);
void instr_SET_5_HL(CPU *cpu, int args);

void instr_SET_6_8(CPU *cpu, int args);
void instr_SET_6_HL(CPU *cpu, int args);

void instr_SET_7_8(CPU *cpu, int args);
void instr_SET_7_HL(CPU *cpu, int args);


#endif 