#ifndef CPU_H
#define CPU_H

#include <stdint.h>

// define the CPU components
typedef struct CPU{
    uint8_t A, B, C, D, E, H, L, F; // 8-bit registers
    uint16_t PC, SP; // 16-bit registers
    int cycles;
} CPU;

void cpu_init(void);

void cpu_cycle(void);

#endif