#ifndef REGISTERS_H
#define REGISTERS_H
#include <stdint.h>

typedef enum registers {
    d0,
    d1,
    d2,
    d3,
    d4,
    d5,
    d6,
    d7,
    a0,
    a1,
    a2,
    a3,
    a4,
    a5,
    a6,
    a7
}registers;


registers get_register(const char* str);
void read_register(registers reg);
void write_register(registers reg, uint32_t data);

#endif
