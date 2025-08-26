#include "memory.h"
#include <stdint.h>
#include <stdio.h>

uint8_t memory[0x10000];
/*
0x0000–0x3FFF : ROM Bank 0 (fixed)
0x4000–0x7FFF : ROM Bank N (switchable)
0x8000–0x9FFF : VRAM
0xA000–0xBFFF : External RAM (on cartridge)
0xC000–0xDFFF : Work RAM
0xFE00–0xFE9F : OAM (sprite attributes)
0xFF00–0xFF7F : I/O registers
0xFF80–0xFFFE : High RAM
0xFFFF        : Interrupt Enable
*/
uint8_t *rom = NULL;
uint32_t size = 0;

int actual_bank;

void load_rom(const char *fileName){
    FILE *f = fopen(fileName, "rb");
    if(!f){
        // couldn't open the hgile
        return;
    }
    fseek(f, 0, SEEK_END);
    size = ftell(f); // file size in bytes
    fseek(f, 0, SEEK_SET);
    rom = malloc(size);
    if(!rom){
        // couldn't separate the memory
        fclose(f);
        return;
    }
    fread(rom, 1, size, f);
    fclose(f);
    load_rom_in_memory();
}

void load_rom_in_memory(void){
    for(int i = 0; i <= 0x3FFF && i < size; i++){
        memory[i] = rom[i];
    }
    for(int i = 0; i <= 0x7FFF; i++){
        if(0x4000 + i < size){
            memory[0x4000 + i] = rom[0x4000 + i];
        }
        else{
            memory[0x4000 + i] = 0xFF;
        }
    }
    actual_bank = 1;
}

void switch_mem_bank(uint8_t bank){
    if(bank = 0) bank = 1;
    if(bank * 0x4000 >= size) return;
    actual_bank = bank;
    //remap the bank to what I want
    uint32_t offset = 0x4000 * bank; // each bank is 16kb, so you jump 16kb * the bank number
    for(int i = 0; i <= 0x3FFF; i++){
        if(offset + i < size){
            memory[0x4000 + i] = rom[offset + i];
        }
        else{
            memory[0x4000 + i] = 0xFF;
        }
    }
}

uint8_t mem_read(uint16_t addr){
    return memory[addr];
}

void mem_write(uint16_t addr, uint8_t data){
    if(addr >= 0x2000 && addr <= 0x3FFF){
        // bank switching
        switch_mem_bank(data & 0x7F); // when we want to switch banks, data is the bank we want to go to
    }
    else{
        memory[addr] = data;
    }
}