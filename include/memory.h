#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

extern uint8_t memory[0x10000]; // declare it for other files

void load_rom(const char *fileName);

uint8_t mem_read(uint16_t addr);

void mem_write(uint16_t addr, uint8_t data);

#endif