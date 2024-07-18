#ifndef COMMANDS_H
#define COMMANDS_H 

#include <stdbool.h>

enum
{
	num_of_cmds = 11,
	cmd_length = 5,
	arg_length = 50,
	max_num_of_args = 10,
	end_address_space = 0x0009fff0,
	end_rom_space = 0x00080000
};

extern bool exit_code;

extern const char commands[num_of_cmds][cmd_length];

void rmp(char args[num_of_cmds][arg_length], int num_of_args);
void cls(char args[num_of_cmds][arg_length], int num_of_args);
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

#endif