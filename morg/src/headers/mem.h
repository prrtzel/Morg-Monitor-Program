#ifndef MEM_H
#define MEM_H

#include <stdint.h>

void read_memory(uint32_t address);
void mem_dump(uint32_t starting_address, uint32_t ending_address);
void write_memory(uint32_t address, const char data);

#endif