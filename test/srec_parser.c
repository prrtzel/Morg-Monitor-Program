#include "srec_parser.h"
#include "conversions.h"
#include "morgio.h"
#include "morglib.h"

#define DBUG
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
			s.data[j] = (char) ascii_hex_to_bin(&srec[i], byte_length);
			j++;
		}
	}
	srec = srec + s.data_length;

	// Set the checksum
	s.checksum = (char) ascii_hex_to_bin(srec, checksum_length);

	// Verify checksum
	if (checksum != s.checksum)
	{
		serial_print("Error: Checksum Failed\n\r");
		s.type = error;
	}

	// Return
	morg_memcpy(srec_struct, &s, sizeof(srecord));
}

uint32_t write_srecord(char* srec)
{
	char* ptr = &srec[0];

	while (true)
	{
		srecord s;
		parse_srec_line(ptr, &s);
		ptr = ptr + s.size;

		if (s.type == error)
		{
			break;
		}
		else if (s.type == s1 || s.type == s2 || s.type == s3)
		{
			// TODO Convert to loading bar
			serial_print("Writing to Memory...\n\r");
			// Write the data to memory
			for (int i = 0; i < s.data_length; i++)
			{
				write_memory(s.address, s.data[i]);
				s.address++;
			}
		}
		else if (s.type == s7 || s.type == s8 || s.type == s9)
		{
			serial_print("Termination record found...\n\r"
				"Data Wrote Successfully\n\r");
			return s.start_address;
		}
		else if (s.type == s0)
		{
			// TODO display program name
		}
		else break;
	}
	serial_print("Error: Parse Failed. Default Start Address now at 0\n\r");
	return 0;
}

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
		sum = (int) ascii_hex_to_bin(&srec_line[i], 2) + sum;
	}
	char checksum = (char) (0xFF - (sum & 0xFF));
	return checksum;
}

void run_srec(uint32_t start_address)
{
	((void (*)(void))start_address)();
	return;
}