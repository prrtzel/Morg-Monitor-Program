#include "conversions.h"
#include "morgio.h"
#include "morglib.h"
#include "commands.h"
#include "mem.h"
#include "registers.h"
#include "srec_parser.h"

// TODO : allow smaller addresses EX: 69 -> 00000069

// ReSharper disable once CppVariableCanBeMadeConstexpr
const char commands[num_of_cmds][cmd_length] = { "rm", "dmp", "wm", "rr", "wr", "ld", "run", "exit", "help", "cls" };

typedef void (*function_pointer)(char args[num_of_cmds][arg_length], int num_of_args);
// ReSharper disable once CppVariableCanBeMadeConstexpr
function_pointer const cmd_ptr[] = { rm, dmp, wm, rr, wr, ld, run, exit_morg, help, cls };

bool exit_code = false;

void cls(char args[num_of_cmds][arg_length], int num_of_args)
{
	serial_print("\033\143");
}

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
		serial_print("Error: Invalid Address. Must be 4 hex bytes long.\n\r");
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
	else if (get_str_length(args[1]) != 8 || get_str_length(args[2]) != 2)
	{
		serial_print("Error: Invalid Address and or Value. Must be 4 hex bytes long.\n\r");
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
	else if (get_str_length(args[1]) != 2)
	{
		serial_print("Error: Invalid Register.\n\r");
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
	else if (get_str_length(args[1]) != 2 || get_str_length(args[2]) != 8)
	{
		serial_print("Error: Invalid Register and or Value. Must be 4 hex bytes long.\n\r");
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
		serial_print("\n\r");
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
		"\n\r\n\rMorg Monitor Program\n\r"
		"'help'\tDisplays this page\n\r"
		"'cls' \tClears the Screen\n\r"
		"'exit'\tExit Morg (you monster)\n\r"
		"'rm'  \tReads Byte at specific location\n\r"
		"'dmp' \tDumps bytes from a memory range\n\r"
		"'wm'  \tWrites a byte to the specified location in memory\n\r"
		"'rr'  \tRead a specified register\n\r"
		"'wr'  \tWrite a long word to a specified register\n\r"
		"'ld'  \tLoad an s-record into memory\n\r"
		"'run' \tRun the s-record\n\r\n\r"
		"ROM Memory: 0x00000000 - 0x0007CFFF\n\r"
		"RAM Memory: 0x00080000 - 0x0009FFF0\n\r\n\r");
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

	split_str_cmd(input_buffer, args);

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
