#include "cpu.h"
#include "memory.h"
#include "instructions.h"
#include <stdio.h>

void reset_cpu_and_memory() {
    cpu_init();      // reset registers
    for (int i = 0; i < 0x10000; i++)
        memory[i] = 0;
}

void run_test(const char* test_name, uint16_t start_addr, uint8_t* program, size_t size) {
    reset_cpu_and_memory();

    // Load program into memory
    for (size_t i = 0; i < size; i++)
        memory[start_addr + i] = program[i];

    cpu.PC = start_addr;

    // Execute each instruction
    for (size_t i = 0; i < size; i++)
        cpu_cycle();

    printf("--- %s ---\n", test_name);
    printf("A=%02X B=%02X C=%02X D=%02X E=%02X H=%02X L=%02X\n",
           cpu.A, cpu.B, cpu.C, cpu.D, cpu.E, cpu.H, cpu.L);
    printf("Flags: Z=%d N=%d H=%d C=%d\n",
           (cpu.F & FLAG_Z) != 0,
           (cpu.F & FLAG_N) != 0,
           (cpu.F & FLAG_H) != 0,
           (cpu.F & FLAG_C) != 0);
    printf("SP=%04X PC=%04X\n\n", cpu.SP, cpu.PC);
}

int main() {
    // Test 1: LD B, 0x42
    uint8_t test1[] = {0x06, 0x42}; // LD B, d8
    run_test("LD B, d8", 0x0100, test1, sizeof(test1));

    // Test 2: INC B
    uint8_t test2[] = {0x04}; // INC B
    run_test("INC B", 0x0100, test2, sizeof(test2));

    // Test 3: DEC B
    uint8_t test3[] = {0x05}; // DEC B
    run_test("DEC B", 0x0100, test3, sizeof(test3));

    // Test 4: LD C, 0x99; LD A, C
    uint8_t test4[] = {0x0E, 0x99, 0x79}; // LD C, d8 ; LD A, C
    run_test("LD C, d8 and LD A, C", 0x0100, test4, sizeof(test4));

    // Add more tests here...
}
