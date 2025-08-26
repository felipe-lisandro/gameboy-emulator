#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

uint8_t mem_read(uint16_t addr);

uint8_t mem_write(uint16_t addr, uint8_t data);

#endif