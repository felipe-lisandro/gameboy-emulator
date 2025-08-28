#include "cpu.h"
#include "memory.h"
#include "instructions.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

CPU cpu;

#define REG16_GET(hi, lo)   (((uint16_t)(hi) << 8) | (lo))
#define REG16_SET(hi, lo, val) \
    do { \
        (hi) = ((val) >> 8) & 0xFF; \
        (lo) = (val) & 0xFF; \
    } while (0)


void cpu_init(void){
    memset(&cpu, 0, sizeof(CPU));
    cpu.cycles = 0;
    cpu.SP = 0xFFFE; // stack pointer initialized at a far point
    cpu.PC = 0x0100; // where the pc is initiallized by rule, after the boot sequence
    // later on the other registers should be initilized to values the boot sequence leaves them in
}

void cpu_cycle(void){
    uint8_t opcode = mem_read(cpu.PC++);
    Instruction instr = InstructionTable[opcode];
    printf("Opcode = %02X, args = %d\n", opcode, instr.args);
    instr.execute(&cpu, instr.args);
    cpu.cycles += InstructionTable[opcode].cycles;
}