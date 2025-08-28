#ifndef CPU_H
#define CPU_H

#include <stdint.h>

// flags in F register
#define FLAG_Z 0x80  // zero
#define FLAG_N 0x40  // subtract
#define FLAG_H 0x20  // half-carry
#define FLAG_C 0x10  // carry

// masks to manipulate bits in F register
#define SET_FLAG(cpu, flag)   ((cpu)->F |= (flag))
#define CLEAR_FLAG(cpu, flag) ((cpu)->F &= ~(flag))
#define GET_FLAG(cpu, flag)   (((cpu)->F & (flag)) != 0)

// define the CPU components
typedef struct CPU{
    uint8_t A, B, C, D, E, H, L, F; // 8-bit registers
    uint16_t PC, SP; // 16-bit registers
    int cycles;
} CPU;

extern CPU cpu;

void cpu_init(void);

void cpu_cycle(void);

// 16-bit register helpers
static inline uint16_t get_HL(CPU *cpu){
    uint16_t reg = REG16_GET(cpu->H, cpu->L);
    return reg;
}
static inline void set_HL(CPU *cpu, uint16_t value){
    REG16_SET(cpu->H, cpu->L, value);
}
static inline uint16_t get_BC(CPU *cpu){
    uint16_t reg = REG16_GET(cpu->B, cpu->C);
    return reg;
}
static inline void set_BC(CPU *cpu, uint16_t value){
    REG16_SET(cpu->B, cpu->C, value);
}
static inline uint16_t get_DE(CPU *cpu){
    uint16_t reg = REG16_GET(cpu->D, cpu->E);
    return reg;
}
static inline void set_DE(CPU *cpu, uint16_t value){
    REG16_SET(cpu->D, cpu->E, value);
}

static inline uint16_t GET_SP(CPU *cpu){
    return cpu->SP;
}

#endif