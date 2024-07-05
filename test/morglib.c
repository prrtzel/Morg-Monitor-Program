#include "morglib.h"
#include <stdbool.h>
#include <stdint.h>
#include "conversions.h"
#include "morgio.h"
#include "srec_parser.h"

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

void morg_memcpy(void *dest, void* src, uint32_t n)
{
	char* c_dest = (char*)dest;
	char* c_src = (char*)src;

	for (uint32_t i = 0; i < n; i++)
	{
		c_dest[i] = c_src[i];
	}
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
	{
		serial_print("Error: Invalid Arguments.\n\rUsage: rm [address]\n\r");
	}
	else if (get_str_length(args[1]) != 8)
	{
		serial_print("Error: Invalid Address. Must be 8 bytes long.\n\r");
	}
	else
	{
		const uint32_t address = ascii_hex_to_bin(args[1], 0);
		if (address > end_address_space)
		{
			serial_print("Error: Address out of range.\n\r");
		}
		else
		{
			read_memory(address);
			serial_print("\n\r\n\r");
		}
	}
}

void dmp(char args[num_of_cmds][arg_length], int num_of_args) {
	if (num_of_args != 3)
	{
		serial_print("Error: Invalid Arguments.\n\rUsage: dmp [start address] [end address]\n\r");
	}
	else if (get_str_length(args[1]) != 8 || get_str_length(args[2]) != 8)
	{
		serial_print("Error: Invalid Address. Must be 8 bytes long.\n\r");
	}
	else
	{
		const uint32_t start_address = ascii_hex_to_bin(args[1], 0);
		const uint32_t end_address = ascii_hex_to_bin(args[2], 0);
		if (start_address > end_address_space || end_address > end_address_space)
		{
			serial_print("Error: Address out of range.\n\r");
		}
		else
		{
			mem_dump(start_address, end_address);
		}
	}
}

void wm(char args[num_of_cmds][arg_length], int num_of_args) {
	if (num_of_args != 3)
	{
		serial_print("Error: Invalid Arguments.\n\rUsage: wm [address] [value]\n\r");
	}
	else if (get_str_length(args[1]) != 8 || get_str_length(args[2]) != 8)
	{
		serial_print("Error: Invalid Address and or Value. Must be 8 bytes long.\n\r");
	}
	else
	{
		const uint32_t address = ascii_hex_to_bin(args[1], 0);
		const char data = ascii_hex_to_bin(args[2], 0);
		if (address > end_address_space || address < end_rom_space)
		{
			serial_print("Error: Address out of range or in ROM.\n\r");
		}
		else
		{
			write_memory(address, data);
		}
	}
}

void rr(char args[num_of_cmds][arg_length], int num_of_args) {
	if (num_of_args != 2)
	{
		serial_print("Error: Invalid Arguments.\n\rUsage: rr [register]\n\r");
	}
	else
	{
		const registers reg = get_register(args[1]);
		read_register(reg);
	}
}

void wr(char args[num_of_cmds][arg_length], int num_of_args) {
	if (num_of_args != 3)
	{
		serial_print("Error: Invalid Arguments.\n\rUsage: wr [register] [value]\n\r");
	}
	else
	{
		const registers reg = get_register(args[1]);
		const uint32_t data = ascii_hex_to_bin(args[2], 0);
		write_register(reg, data);
	}
}

uint32_t srec_start_address = 0;
void ld(char args[num_of_cmds][arg_length], int num_of_args) {
	if (num_of_args != 1)
		serial_print("Error: Invalid Arguments.\n\rUsage: ld\n\r");
	else
	{
		serial_print("Enter the S-Record:\n\r");
		get_string();
		srec_start_address = write_srecord(input_buffer);
	}
}

void run(char args[num_of_cmds][arg_length], int num_of_args) {
	if (num_of_args != 1)
	{
		serial_print("Error: Invalid Arguments.\n\rUsage: run\n\r");
	}
	else
	{
		run_srec(srec_start_address);
	}
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
		"'rm'\t  Reads Byte at specific location\n\r"
		"'dmp'\t Dumps bytes from a memory range\n\r"
		"'wm'\t  Writes a byte to the specified location in memory\n\r"
		"'rr'\t  Read a specified register\n\r"
		"'wr'\t  Write a long word to a specified register\n\r"
		"'ld'\t  Load an s-record into memory\n\r"
		"'run'\t Run the s-record\n\r\n\r");
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
		serial_print("Error: Invalid Command.\n\r");
}
#pragma endregion

#pragma region Memory Functions
// Memory and Register -----------------------------------------------------------------------------------------------------

void read_memory(uint32_t address) { //NOLINT
	char* address_pointer = (char*)address; //NOLINT
	binary_to_ascii_hex(*address_pointer, output_buffer, hex_byte_length);
	serial_print(output_buffer);
}

void mem_dump(uint32_t starting_address, uint32_t ending_address)
{
	int j = 1;
	for (uint32_t i = 0; i <= ending_address; i++) 
	{
		read_memory(starting_address + i);
		serial_print(" ");
		if (j == 16) 
		{
			serial_print("\n\r");
			j = 0;
		}
		j++;
	}
	serial_print("\n\r\n\r");
}

void write_memory(uint32_t address, const char data) {
	char* address_pointer = (char*)address; //NOLINT
	*address_pointer = data;
}

#pragma endregion

#pragma region Register Functions
// Register -----------------------------------------------------------------------------------------------------

const struct {
	const char* str;
	registers reg;
} register_map[] = {
	{ "d0", d0 },
	{ "d1", d1 },
	{ "d2", d2 },
	{ "d3", d3 },
	{ "d4", d4 },
	{ "d5", d5 },
	{ "d6", d6 },
	{ "d7", d7 },
	{ "a0", a0 },
	{ "a1", a1 },
	{ "a2", a2 },
	{ "a3", a3 },
	{ "a4", a4 },
	{ "a5", a5 },
	{ "a6", a6 },
	{ "a7", a7 }
};

registers get_register(const char* str) {
	for (int i = 0; i < 16; i++) {
		if (cmp_str(str, register_map[i].str)) {
			return register_map[i].reg;
		}
	}
	serial_print("Error: No such register, defaulting to d0");
	return d0;
}

static uint32_t register_result = 0;
void read_register(registers reg) {
	register_result = 0;
	switch (reg) {
	case d0:
		__asm("move.l %d0, register_result");
		break;
	case d1:
		__asm("move.l %d1, register_result");
		break;
	case d2:
		__asm("move.l %d2, register_result");
		break;
	case d3:
		__asm("move.l %d3, register_result");
		break;
	case d4:
		__asm("move.l %d4, register_result");
		break;
	case d5:
		__asm("move.l %d5, register_result");
		break;
	case d6:
		__asm("move.l %d6, register_result");
		break;
	case d7:
		__asm("move.l %d7, register_result");
		break;
	case a0:
		__asm("move.l %a0, register_result");
		break;
	case a1:
		__asm("move.l %a1, register_result");
		break;
	case a2:
		__asm("move.l %a2, register_result");
		break;
	case a3:
		__asm("move.l %a3, register_result");
		break;
	case a4:
		__asm("move.l %a4, register_result");
		break;
	case a5:
		__asm("move.l %a5, register_result");
		break;
	case a6:
		__asm("move.l %a6, register_result");
		break;
	case a7:
		__asm("move.l %a7, register_result");
		break;
	}

	binary_to_ascii_hex(register_result, output_buffer, hex_long_length);
	serial_print(output_buffer);
	serial_print("\n\r\n\r");
}

static uint32_t reg_write_value = 0;
void write_register(registers reg, uint32_t data) {
	reg_write_value = data;
	switch (reg) {
	case d0:
		__asm("move.l reg_write_value, %d0");
		break;
	case d1:
		__asm("move.l reg_write_value, %d1");
		break;
	case d2:
		__asm("move.l reg_write_value, %d2");
		break;
	case d3:
		__asm("move.l reg_write_value, %d3");
		break;
	case d4:
		__asm("move.l reg_write_value, %d4");
		break;
	case d5:
		__asm("move.l reg_write_value, %d5");
		break;
	case d6:
		__asm("move.l reg_write_value, %d6");
		break;
	case d7:
		__asm("move.l reg_write_value, %d7");
		break;
	case a0:
		__asm("move.l reg_write_value, %a0");
		break;
	case a1:
		__asm("move.l reg_write_value, %a1");
		break;
	case a2:
		__asm("move.l reg_write_value, %a2");
		break;
	case a3:
		__asm("move.l reg_write_value, %a3");
		break;
	case a4:
		__asm("move.l reg_write_value, %a4");
		break;
	case a5:
		__asm("move.l reg_write_value, %a5");
		break;
	case a6:
		__asm("move.l reg_write_value, %a6");
		break;
	case a7:
		__asm("move.l reg_write_value, %a7");
		break;
	}
}

#pragma endregion
