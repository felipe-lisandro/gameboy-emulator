#include "helper.h"

void inc8(uint8_t *reg){
    *reg++;
}

void dec8(uint8_t *reg){
    *reg--;
}

void add8_8(uint8_t *regA, uint8_t *regB){
    *regA += *regB;
}
