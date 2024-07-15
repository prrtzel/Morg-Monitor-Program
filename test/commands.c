#include "conversions.h"
#include "morgio.h"
#include "morglib.h"
#include "commands.h"
#include "mem.h"
#include "registers.h"




/**
 * Length/Size Definitions for S-Record items
 */
enum srec_lengths
{
	type_length = 2,
	byte_count_length = 2,
	address_16 = 4,
	address_24 = 6,
	address_32 = 8,
	checksum_length = 2,
	byte_length = 2
};

/**
 * S-Record Types
 */
typedef enum srecord_type {
	s0,
	s1,
	s2,
	s3,
	s7,
	s8,
	s9,
	error,
	num_of_types
}srecord_type;

/**
 * Struct defining an S-Record
 along with useful information
 */
typedef struct srecord {
	srecord_type type;
	char byte_count;
	uint32_t address;
	char data[251];
	char checksum;
	uint32_t start_address;
	char size;
	char data_length;
} srecord;


const struct
{
	const char* srec_type_str;
	srecord_type srec_type;
} srec_map[] = {
	{"S0", s0},
	{"S1", s1},
	{"S2", s2},
	{"S3", s3},
	{"S7", s7},
	{"S8", s8},
	{"S9", s9}
};



srecord_type get_srec_type(char* srec)
{
	char buffer[3];
	morg_memcpy(buffer, srec, 2);
	buffer[2] = '\0';

	for (int i = 0; i < num_of_types; i++)
	{
		if (cmp_str(buffer, srec_map[i].srec_type_str))
		{
			return srec_map[i].srec_type;
		}
	}
	serial_print("Invalid Srecord type\n\r");
	return error;
}

char get_checksum(char* srec_line, const int length)
{
	int sum = 0;

	// Skip S-type first two chars
	for (int i = 2; i < length - checksum_length; i += 2)
	{
		sum = (int)ascii_hex_to_bin(&srec_line[i], 2) + sum;
	}
	char checksum = (char)(0xFF - (sum & 0xFF));
	return checksum;
}

void run_srec(uint32_t start_address)
{
	((void (*)(void))start_address)();
	return;
}

void disp_srec_loading_bar(int num_of_records, int num_of_srec_written)
{
	char num_of_records_str[3];
	binary_to_ascii_hex(num_of_records, num_of_records_str, 2);
	num_of_records_str[2] = '\0';

	char num_of_srec_written_str[3];
	binary_to_ascii_hex(num_of_srec_written, num_of_srec_written_str, 2);
	num_of_srec_written_str[2] = '\0';
	serial_print(num_of_srec_written_str);
	serial_print(" out of ");
	serial_print(num_of_records_str);
	serial_print(" Records Written\n\r");
}


// ReSharper disable once CppParameterMayBeConstPtrOrRef
void parse_srec_line(char* srec, srecord* srec_struct)
{
	srecord s;

	// Set the type 
	s.type = get_srec_type(srec);
	srec = srec + type_length;

	// Set the byte count
	s.byte_count = (unsigned char)ascii_hex_to_bin(srec, byte_count_length);
	srec = srec + byte_count_length;

	// Calculate total size
	s.size = (char)type_length + byte_count_length + (s.byte_count * 2);

	// Calculate checksum
	char checksum = get_checksum(srec - type_length - byte_count_length, s.size);

	// Set the Address and Data Length
	switch (s.type)
	{
		// 16-bit Header
	case s0:
		morg_memcpy(srec_struct, &s, sizeof(srecord));
		return;
		// 16-bit Data
	case s1:
		s.data_length = (s.byte_count - 3) * 2;
		s.address = ascii_hex_to_bin(srec, address_16);
		srec = srec + address_16;
		break;
		// 24-bit Data
	case s2:
		s.data_length = (s.byte_count - 4) * 2;
		s.address = ascii_hex_to_bin(srec, address_24);
		srec = srec + address_24;
		break;
		// 32-bit Data
	case s3:
		s.data_length = (s.byte_count - 5) * 2;
		s.address = ascii_hex_to_bin(srec, address_32);
		srec = srec + address_32;
		break;
		// 32-bit Termination
	case s7:
		s.data_length = 0;
		s.start_address = ascii_hex_to_bin(srec, address_32);
		srec = srec + address_32;
		break;
		// 24-bit Termination
	case s8:
		s.data_length = 0;
		s.start_address = ascii_hex_to_bin(srec, address_24);
		srec = srec + address_24;
		break;
		// 16-bit Termination
	case s9:
		s.data_length = 0;
		s.start_address = ascii_hex_to_bin(srec, address_16);
		srec = srec + address_16;
		break;
	case error:
		morg_memcpy(srec_struct, &s, sizeof(srecord));
		return;
	default:
		s.type = error;
		morg_memcpy(srec_struct, &s, sizeof(srecord));
		return;
	}

	// Set the data to the s-record structure
	if (s.type == s1 || s.type == s2 || s.type == s3)
	{
		int j = 0;
		for (int i = 0; i < s.data_length; i += 2)
		{
			s.data[j] = (char)ascii_hex_to_bin(&srec[i], byte_length);
			j++;
		}
	}
	srec = srec + s.data_length;

	// Set the checksum
	s.checksum = (char)ascii_hex_to_bin(srec, checksum_length);

	// Verify checksum
	if (checksum != s.checksum)
	{
		serial_print("Error: Checksum Failed\n\r");
		s.type = error;
	}

	if (s.address < end_rom_space || s.address > end_address_space)
	{
		serial_print("Error: S-Record Address Outside of Valid Address Space Failed\n\r");
		s.type = error;
	}

	// Return
	morg_memcpy(srec_struct, &s, sizeof(srecord));
}

uint32_t write_srecord(char* srec)
{
	//char* ptr = &srec[0];

	//// Loading bar
	//int num_of_records = 0;
	//int num_of_srec_written = 0;

	//// Get the number of records
	//while (*srec != '\0')
	//{
	//	if (*srec == 'S')
	//		num_of_records++;
	//	srec++;
	//}
	//char num_of_records_str[3];
	//binary_to_ascii_hex(num_of_records, num_of_records_str, 2);
	//num_of_records_str[2] = '\0';
	//serial_print(num_of_records_str);
	//serial_print(" Records Found\n\r\n\r");

	//for (int i = 0; i < num_of_records; i++)
	//{
	//	srecord s;
	//	parse_srec_line(ptr, &s);
	//	ptr = ptr + s.size;

	//	if (s.type == error)
	//	{
	//		serial_print("Error: Parse Failed. Default Start Address now at 0\n\r");
	//		break;
	//	}
	//	else if (s.type == s1 || s.type == s2 || s.type == s3)
	//	{
	//		num_of_srec_written++;
	//		disp_srec_loading_bar(num_of_records, num_of_srec_written);
	//		// Write the data to memory
	//		for (int i = 0; i < s.data_length; i++)
	//		{
	//			write_memory(s.address, s.data[i]);
	//			s.address++;
	//		}
	//	}
	//	else if (s.type == s7 || s.type == s8 || s.type == s9)
	//	{
	//		num_of_srec_written++;
	//		disp_srec_loading_bar(num_of_records, num_of_srec_written);
	//		serial_print("\n\rTermination record found...\n\r"
	//			"Data Wrote Successfully\n\rUse the 'run' command to run the program\n\r");
	//		return s.start_address;
	//	}
	//	else if (s.type == s0)
	//	{
	//		num_of_srec_written++;
	//		disp_srec_loading_bar(num_of_records, num_of_srec_written);
	//		// TODO display program name
	//	}
	//	else break;
	//}
	////serial_print("                                                   \n\r");
	return 0;
}











//-----------------------------------------------------------------------










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
		"Morg Monitor Program\n\r"
		"'help'\tDisplays this page\n\r"
		"'cls' \tClears the Screen\n\r"
		"'exit'\tExit Morg (you monster)\n\r"
		"'rm'  \tReads Byte at specific location\n\r"
		"'dmp' \tDumps bytes from a memory range\n\r"
		"'wm'  \tWrites a byte to the specified location in memory\n\r"
		"'rr'  \tRead a specified register\n\r"
		"'wr'  \tWrite a long word to a specified register\n\r"
		"'ld'  \tLoad an s-record into memory\n\r"
		"'run' \tRun the s-record\n\r\n\r");
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
