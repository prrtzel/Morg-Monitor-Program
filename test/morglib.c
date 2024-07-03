#include "morglib.h"
#include <stdbool.h>
#include <stdint.h>
#include "conversions.h"
#include "morgio.h"

#pragma region Library Functions
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
#pragma endregion

#pragma region Command Functions
// Commands ---------------------------------------------------------------------------------------------------

// ReSharper disable once CppVariableCanBeMadeConstexpr
const char commands[num_of_cmds][cmd_length] = { "rm", "dmp", "wm", "rr", "wr", "ld", "run", "exit", "help" };

typedef void (*function_pointer)(char args[num_of_cmds][arg_length], int num_of_args);
// ReSharper disable once CppVariableCanBeMadeConstexpr
function_pointer const cmd_ptr[] = { rm, dmp, wm, rr, wr, ld, run, exit_morg, help };

bool exit_code = false;

void rm(char args[num_of_cmds][arg_length], int num_of_args) {
	if (num_of_args != 2)
		serial_print("Error: Invalid Arguments.\n\rUsage: rm [address]\n\r");
	else if (get_str_length(args[1]) != 8)
		serial_print("Error: Invalid Address. Must be 8 bytes long.\n\r");
	else
	{
		const uint32_t address = ascii_hex_to_bin(args[1]);
		if (address > 0x0011f000)
		{
			serial_print("Error: Address out of range.\n\r");
		}
		else
		{
			read_memory(address);
		}
	}
}
void dmp(char args[num_of_cmds][arg_length], int num_of_args) {
	if (num_of_args != 3)
		serial_print("Error: Invalid Arguments.\n\rUsage: dmp [start address] [end address]\n\r");
	else if (get_str_length(args[1]) != 8 || get_str_length(args[2]) != 8)
		serial_print("Error: Invalid Address. Must be 8 bytes long.\n\r");
}
void wm(char args[num_of_cmds][arg_length], int num_of_args) {
	if (num_of_args != 3)
		serial_print("Error: Invalid Arguments.\n\rUsage: wm [address] [value]\n\r");
	else if (get_str_length(args[1]) != 8 || get_str_length(args[2]) != 8)
		serial_print("Error: Invalid Address and or Value. Must be 8 bytes long.\n\r");
}
void rr(char args[num_of_cmds][arg_length], int num_of_args) {
	if (num_of_args != 2)
		serial_print("Error: Invalid Arguments.\n\rUsage: rr [register]\n\r");
}
void wr(char args[num_of_cmds][arg_length], int num_of_args) {
	if (num_of_args != 3)
		serial_print("Error: Invalid Arguments.\n\rUsage: wr [register] [value]\n\r");
}
void ld(char args[num_of_cmds][arg_length], int num_of_args) {
	if (num_of_args != 1)
		serial_print("Error: Invalid Arguments.\n\rUsage: ld\n\r");
}
void run(char args[num_of_cmds][arg_length], int num_of_args) {
	if (num_of_args != 1)
		serial_print("Error: Invalid Arguments.\n\rUsage: run\n\r");
}

void exit_morg(char args[num_of_cmds][arg_length], int num_of_args)
{
	serial_print("Program Ended");
	exit_code = true;
}

void help(char args[num_of_cmds][arg_length], int num_of_args)
{
	serial_print(
"Morg Monitor Program\n\r"
  "'help'\tDisplays this page\n\r"
  "'exit'\tExit Morg (you monster)\n\r"
  "'rm'\tReads Byte at specific location\n\r"
  "'dmp'\tDumps bytes from a memory range\n\r"
  "'wm'\tWrites a byte to the specified location in memory\n\r"
  "'rr'\tRead a specified register\n\r"
  "'wr'\tWrite a long word to a specified register\n\r"
  "'ld'\tLoad an s-record into memory\n\r"
  "'run'\tRun the s-record\n\r");
}

void parse_cmd(void)
{
	char args[num_of_cmds][arg_length];

	// clear the args buffer because = {0} doesn't work :(
	for (int i = 0; i < num_of_cmds; i++) {
		for (int j = 0; j < arg_length; j++) {
			args[i][j] = 0;
		}
	}

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

	bool is_valid_cmd = false;

	//Match the key and call the respective cmd
	for (int i = 0; i < num_of_cmds; i++)
	{
		if (cmp_str(args[0], commands[i]) == true)
		{
			cmd_ptr[i](args, num_of_args);
			is_valid_cmd = true;
			break;
		}
	}

	if (is_valid_cmd == false)
		serial_print("Error: Invalid Command.\n");
}
#pragma endregion

#pragma region Memory and Register Functions
// Memory and Register -----------------------------------------------------------------------------------------------------

void read_memory(uint32_t address) { //NOLINT
	char* address_pointer = (char*)address; //NOLINT
	binary_to_ascii_hex(*address_pointer, output_buffer, hex_byte_length);
	serial_print(output_buffer);
	serial_print("\n\r");
}

//void mem_dump(const char *starting_address, const char* ending_address) {
//	int j = 1;
//	for (int i = 0; i <= (int)ending_address; i++) {
//		read_memory(starting_address + i);
//		serial_print(" ");
//		if (j == 16) {
//			serial_print("\n\r");
//			j = 0;
//		}
//		j++;
//	}
//	serial_print("\n\r");
//}
//
//void write_memory(char *address, const char data) {
//	char* address_pointer = address;
//	*address_pointer = data;
//	serial_print("Data Wrote Successfully\n\r");
//}

#pragma endregion

#pragma region S-Record Functions
// S-Record -----------------------------------------------------------------------------------------------------
#pragma endregion
