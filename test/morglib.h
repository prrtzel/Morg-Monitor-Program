#ifndef MORGLIB_H
#define MORGLIB_H //NOLINT
#include <stdbool.h>


enum command_lengths
{
	num_of_cmds = 8,
	cmd_length = 4,
	arg_length = 50,
	max_num_of_args = 10
};

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

void rm(char args[num_of_cmds][arg_length], int num_of_args);
void dmp(char args[num_of_cmds][arg_length], int num_of_args);
void wm(char args[num_of_cmds][arg_length], int num_of_args);
void rr(char args[num_of_cmds][arg_length], int num_of_args);
void wr(char args[num_of_cmds][arg_length], int num_of_args);
void ld(char args[num_of_cmds][arg_length], int num_of_args);
void run(char args[num_of_cmds][arg_length], int num_of_args);

extern void (*fn[7])(char args[num_of_cmds][arg_length], int num_of_args);

void parse_cmd(void);

#endif
