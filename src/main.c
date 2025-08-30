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

    uint16_t end = size + start_addr;
    // Load program into memory
    for (size_t i = 0; i < size; i++)
        memory[start_addr + i] = program[i];
    
    cpu.PC = start_addr;

    // Execute each instruction
    while(cpu.PC < end) cpu_cycle();


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

        // Test 5: ADD A, B (with B=0x10)
    uint8_t test5[] = {0x06, 0x10, // LD B, 0x10
                        0x3E, 0x20, // LD A, 0x20
                        0x80};      // ADD A, B
    run_test("ADD A, B", 0x0100, test5, sizeof(test5));

    // Test 6: SUB A, C (with A=0x50, C=0x30)
    uint8_t test6[] = {0x0E, 0x30, // LD C, 0x30
                        0x3E, 0x50, // LD A, 0x50
                        0x91};      // SUB A, C
    run_test("SUB A, C", 0x0100, test6, sizeof(test6));

    // Test 7: PUSH and POP
    uint8_t test7[] = {0x3E, 0x12, // LD A, 0x12
                        0x06, 0x34, // LD B, 0x34
                        0xC5,       // PUSH BC
                        0x06, 0x00, // LD B, 0x00
                        0xC1};      // POP BC
    run_test("PUSH BC / POP BC", 0x0100, test7, sizeof(test7));

    // Test 8: Combined arithmetic and stack
    uint8_t test8[] = {0x3E, 0x05, // LD A, 0x05
                        0x06, 0x03, // LD B, 0x03
                        0x80,       // ADD A, B
                        0xC5,       // PUSH BC
                        0x3E, 0x00, // LD A, 0x00
                        0xC1,       // POP BC
                        0x81};      // ADD A, C (A = A + C from popped BC)
    run_test("ADD + PUSH/POP + ADD", 0x0100, test8, sizeof(test8));

    // Test 9: Stack operations with SP
    uint8_t test9[] = {0x31, 0xFF, 0xFF, // LD SP, 0xFFFF
                        0x3E, 0xAA,       // LD A, 0xAA
                        0xF5,             // PUSH AF
                        0x3E, 0x00,       // LD A, 0x00
                        0xF1};            // POP AF
    run_test("PUSH AF / POP AF with SP check", 0x0100, test9, sizeof(test9));

    uint8_t test10[] = {
    /*0x31, 0xFF, 0xF0,*/   // LD SP, 0xFFF0
    0x01, 0x12, 0x34,   // LD BC, 0x3412
    0x11, 0x56, 0x78,   // LD DE, 0x7856
    0x21, 0x9A, 0xBC,   // LD HL, 0xBC9A
    0x3E, 0xDE,          // LD A, 0xDE
    0xF5,                // PUSH AF
    0xC5,                // PUSH BC
    0xD5,                // PUSH DE
    0xE5,                // PUSH HL
    0xF1,                // POP AF
    0xC1,                // POP BC
    0xD1,                // POP DE
    0xE1                 // POP HL
    };
    run_test("Multiple PUSH/POP stress test", 0x0100, test10, sizeof(test10));
}
