#ifndef CPU_INSTRUCTIONS_H
#define CPU_INSTRUCTIONS_H

#include "cpu.h"

// all instruction function prototypes
void instr_NOP(CPU *cpu, int args);

void instr_INC_8(CPU *cpu, int args);
void instr_INC_16(CPU *cpu, int args);
void instr_INC_HL(CPU *cpu, int args);

void instr_DEC_8(CPU *cpu, int args);
void instr_DEC_16(CPU *cpu, int args);
void instr_DEC_HL(CPU *cpu, int args);

void instr_ADD_A_8(CPU *cpu, int args);
void instr_ADD_A_HL(CPU *cpu, int args);
void instr_ADD_A_D8(CPU *cpu, int args);
void instr_ADD_HL_16(CPU *cpu, int args);

void instr_ADC_A_8(CPU *cpu, int args);
void instr_ADC_A_HL(CPU *cpu, int args);
void instr_ADC_A_D8(CPU *cpu, int args);

void instr_SUB_A_8(CPU *cpu, int args);
void instr_SUB_HL(CPU *cpu, int args);
void instr_SUB_D8(CPU *cpu, int args);

void instr_SBC_A_8(CPU *cpu, int args);
void instr_SBC_HL(CPU *cpu, int args);
void instr_SBC_A_D8(CPU *cpu, int args);

void instr_AND_8(CPU *cpu, int args);
void instr_AND_HL(CPU *cpu, int args);
void instr_AND_D8(CPU *cpu, int args);

void instr_OR_8(CPU *cpu, int args);
void instr_OR_HL(CPU *cpu, int args);
void instr_OR_D8(CPU *cpu, int args);

void instr_XOR_8(CPU *cpu, int args);
void instr_XOR_HL(CPU *cpu, int args);
void instr_XOR_D8(CPU *cpu, int args);

void instr_CP_8(CPU *cpu, int args);
void instr_CP_HL(CPU *cpu, int args);
void instr_CP_D8(CPU *cpu, int args);

void instr_LD_8_U8(CPU *cpu, int args);
void instr_LD_16_U16(CPU *cpu, int args);

void instr_LD_A_8(CPU *cpu, int args);
void instr_LD_B_8(CPU *cpu, int args);
void instr_LD_C_8(CPU *cpu, int args);
void instr_LD_D_8(CPU *cpu, int args);
void instr_LD_E_8(CPU *cpu, int args);
void instr_LD_H_8(CPU *cpu, int args);
void instr_LD_L_8(CPU *cpu, int args);

void instr_LD_8_HL(CPU *cpu, int args);
void instr_LD_HL_8(CPU *cpu, int args);

void instr_PUSH_16(CPU *cpu, int args);
void instr_POP_16(CPU *cpu, int args);

void instr_JR_s8(CPU *cpu, int args);
void instr_JR_NZ_s8(CPU *cpu, int args);
void instr_JR_Z_s8(CPU *cpu, int args);
void instr_JR_NC_s8(CPU *cpu, int args);
void instr_JR_C_s8(CPU *cpu, int args);

void instr_CPL(CPU *cpu, int args);
void instr_SCF(CPU *cpu, int args);
void instr_CCF(CPU *cpu, int args);

void instr_RST_N(CPU *cpu, int args);

void instr_RET(CPU *cpu, int args);
void instr_RET_IF(CPU *cpu, int args);

void instr_JP_A16(CPU *cpu, int args);
void instr_JP_IF_A16(CPU *cpu, int args);
void instr_JP_HL(CPU *cpu, int args);

void instr_CALL_A16(CPU *cpu, int args);
void instr_CALL_IF_A16(CPU *cpu, int args);

#endif 