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

void cpu_init(void);

void cpu_cycle(void);

#endif