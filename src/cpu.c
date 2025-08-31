#include "cpu.h"
#include "memory.h"
#include "instructions.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

CPU cpu;

void cpu_init(void){
    memset(&cpu, 0, sizeof(CPU));
    cpu.cycles = 0;
    cpu.SP = 0xFFFE; // stack pointer initialized at a far point
    cpu.PC = 0x0100; // where the pc is initiallized by rule, after the boot sequence
    // later on the other registers should be initilized to values the boot sequence leaves them in
}

void cpu_cycle(void){
    uint8_t opcode = mem_read(cpu.PC++);
    if(opcode != 0xCB){
        Instruction instr = InstructionTable[opcode];
        printf("Opcode = %02X, args = %d\n", opcode, instr.args);
        instr.execute(&cpu, instr.args);
        // printf("A: %02X B: %02X C: %02X D: %02X E: %02X H: %02X L: %02X\nZ: %02X C: %02X N: %02X H: %02X\n", cpu.A, cpu.B, cpu.C, cpu.D, cpu.E, cpu.H, cpu.L, GET_FLAG(&cpu, FLAG_Z), GET_FLAG(&cpu, FLAG_C), GET_FLAG(&cpu, FLAG_N), GET_FLAG(&cpu, FLAG_H));
        cpu.cycles += InstructionTable[opcode].cycles;
    }
    else{
        uint8_t opcodeCB = mem_read(cpu.PC++);
        Instruction instr = CBInstructionTable[opcodeCB];
        // printf("Opcode = 0xCB%02X, args = %d\n", opcode, instr.args);
        instr.execute(&cpu, instr.args);
        cpu.cycles += InstructionTable[opcodeCB].cycles;
    }
    if(cpu.setImeNext){
        cpu.IME = 1;
        cpu.setImeNext = 0;
    }
}