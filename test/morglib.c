#include "morglib.h"
#include <stdbool.h>

#include "conversions.h"
#include "morgio.h"

// ReSharper disable once CppVariableCanBeMadeConstexpr
const char commands[num_of_cmds][cmd_length] = { "rm", "dmp", "wm", "rr", "wr", "ld", "run" };

void clear_buffer(char* buffer, const int size) {
	for (int i = 0; i < size; i++) {
		*buffer++ = 0;
	}
}

int get_str_length(const char* string) {
	int i = 0;
	while (string[i] != '\0') {
		i++;
	}
	return i;
}

void split_str(const char* str, char split_string_array[num_of_cmds][arg_length]) {
	const int str_length = get_str_length(str);

	// counters and index vars
	int j = 0;
	int ctr = 0;

	// loop through each char in input string
	for (int i = 0; i <= str_length; i++)
	{
		if (str[i] == ' ' || str[i] == '\0')
		{
			split_string_array[ctr][j] = '\0';
			ctr++; // move on to the next word
			j = 0;
		}
		else
		{
			split_string_array[ctr][j] = str[i];
			j++;
		}
	}
}

bool cmp_str(const char* str1, const char* str2) {
	while (*str1 != '\0' || *str2 != '\0') {
		if (*str1 == *str2) {
			str1++;
			str2++;
		}
		else if ((*str1 != '\0' && *str2 == '\0') ||
			(*str1 == '\0' && *str2 != '\0') ||
			(*str1 != *str2)) {
			return false;
		}
	}
	return true;
}

void rm(char args[num_of_cmds][arg_length], int num_of_args) {
	if (num_of_args != 2)
		serial_print("Error: Invalid Arguments.\nUsage: rm [address]\n");
	else if (get_str_length(args[1]) != 8)
		serial_print("Error: Invalid Address. Must be 8 bytes long.\n");
	return;
}
void dmp(char args[num_of_cmds][arg_length], int num_of_args) {
	if (num_of_args != 3)
		serial_print("Error: Invalid Arguments.\nUsage: dmp [start address] [end address]\n");
	else if (get_str_length(args[1]) != 8 || get_str_length(args[2]) != 8)
		serial_print("Error: Invalid Address. Must be 8 bytes long.\n");
	return;
}
void wm(char args[num_of_cmds][arg_length], int num_of_args) {
	if (num_of_args != 3)
		serial_print("Error: Invalid Arguments.\nUsage: wm [address] [value]\n");
	else if (get_str_length(args[1]) != 8 || get_str_length(args[2]) != 8)
		serial_print("Error: Invalid Address and or Value. Must be 8 bytes long.\n");
	return;
}
void rr(char args[num_of_cmds][arg_length], int num_of_args) {
	if (num_of_args != 2)
		serial_print("Error: Invalid Arguments.\nUsage: rr [register]\n");
	return;
}
void wr(char args[num_of_cmds][arg_length], int num_of_args) {
	if (num_of_args != 3)
		serial_print("Error: Invalid Arguments.\nUsage: wr [register] [value]\n");
	return;
}
void ld(char args[num_of_cmds][arg_length], int num_of_args) {
	if (num_of_args != 1)
		serial_print("Error: Invalid Arguments.\nUsage: ld\n");
	else
		serial_print("ld");
	return;
}
void run(char args[num_of_cmds][arg_length], int num_of_args) {
	if (num_of_args != 1)
		serial_print("Error: Invalid Arguments.\nUsage: run\n");
	else
		serial_print("run");
	return;
}

void (*fn[7])(char args[num_of_cmds][arg_length], int num_of_args) =
{ &rm, &dmp, &wm, &rr, &wr, &ld, &run };

void parse_cmd(void)
{

	// Declare and clear args, cant use = {0} because that file is missing lol
	char args[num_of_cmds][arg_length];
	for (int i = 0; i < num_of_cmds; i++) {
		for (int j = 0; j < arg_length; j++) {
			args[i][j] = 0;
		}
	}

	get_string();
	split_str(input_buffer, args);

	//find number of args
	int num_of_args = 0;
	for (int i = 0; i < num_of_cmds; i++)
	{
		if (args[i][0] != '\0')
			num_of_args++;
		else
			break;
	}

	binary_to_ascii_hex(num_of_args, output_buffer, 1);
	serial_print(output_buffer);
	serial_print("\n\r");
	clear_buffer(output_buffer, output_buffer_size);

	//get rid of /n
	for (int i = 0; i < arg_length; i++)
	{
		if (args[num_of_args - 1][i] == '\n')
		{
			serial_print("replaced n with 0");
			args[num_of_args - 1][i] = '\0';
			break;
		}
	}

	bool is_valid_cmd = false;

	serial_print(args[0]);

	//Match the key and call the respective cmd
	for (int i = 0; i < num_of_cmds; i++)
	{
		if (cmp_str(args[0], commands[i]) == true)
		{
			serial_print("\n\rcmp true\n\r");
			(*fn[i])(args, num_of_args);
			serial_print("control returned");
			is_valid_cmd = true;
			break;
		}
	}

	if (is_valid_cmd == false)
		serial_print("Error: Invalid Command.\n");
}