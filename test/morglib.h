#ifndef MORGLIB_H
#define MORGLIB_H //NOLINT
#include <stdbool.h>
#include <stdint.h>


enum
{
	num_of_cmds = 9,
	cmd_length = 5,
	arg_length = 50,
	max_num_of_args = 10,
	end_address_space = 0x0011f000,
	end_rom_space = 0x00100000
};

typedef enum registers{
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

/**
 * @brief clears a given array/buffer to 0
 *
 * @param buffer pointer to buffer thy wish to clear
 * @param size size of thy buffer (it does matter in this case)
 */
extern void clear_buffer(char* buffer, const int size);

extern const char commands[num_of_cmds][cmd_length];

int get_str_length(const char* string);

void split_str(const char* str, char split_string_array[num_of_cmds][arg_length]);

bool cmp_str(const char* str1, const char* str2);

extern bool exit_code;

void rm(char args[num_of_cmds][arg_length], int num_of_args);
void dmp(char args[num_of_cmds][arg_length], int num_of_args);
void wm(char args[num_of_cmds][arg_length], int num_of_args);
void rr(char args[num_of_cmds][arg_length], int num_of_args);
void wr(char args[num_of_cmds][arg_length], int num_of_args);
void ld(char args[num_of_cmds][arg_length], int num_of_args);
void run(char args[num_of_cmds][arg_length], int num_of_args);
void exit_morg(char args[num_of_cmds][arg_length], int num_of_args);
void help(char args[num_of_cmds][arg_length], int num_of_args);

void parse_cmd(void);


void read_memory(uint32_t address);

void mem_dump(uint32_t starting_address, uint32_t ending_address);

void write_memory(uint32_t address, const char data);

registers get_register(const char* str);

void read_register(registers reg);

void write_register(registers reg, uint32_t data);
#endif
