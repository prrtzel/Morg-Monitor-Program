#include "srec_parser.h"
#include "conversions.h"
#include "main.h"
#include "morgio.h"
#include "morglib.h"

// ReSharper disable once CppParameterMayBeConstPtrOrRef
srecord parse_srec_line(char* srec)
{
	srecord s;

	// Set the type and byte count
	s.type = get_srec_type(srec);
	srec = srec + type_length;
	s.byte_count = (unsigned char)ascii_hex_to_bin(srec, byte_count_length);
	srec = srec + byte_count_length;
	s.size = type_length + byte_count_length + s.byte_count;

	int sum = s.byte_count;

	// Set the Address and Data Length
	switch (s.type)
	{
		// 16-bit Header
		case s0:
			return s;
		// 16-bit Data
		case s1:
			s.data_length = s.byte_count - address_16 - checksum_length;
			s.address = ascii_hex_to_bin(srec, address_16);
			sum = find_address_sum(srec, address_16);
			srec = srec + address_16;
			break;
		// 24-bit Data
		case s2:
			s.data_length = s.byte_count - address_24 - checksum_length;
			s.address = ascii_hex_to_bin(srec, address_24);
			sum = find_address_sum(srec, address_24);
			srec = srec + address_24;
			break;
		// 32-bit Data
		case s3:
			s.data_length = s.byte_count - address_32 - checksum_length;
			s.address = ascii_hex_to_bin(srec, address_32);
			sum = find_address_sum(srec, address_32);
			srec = srec + address_32;
			break;
		// 32-bit Termination
		case s7:
			s.start_address = ascii_hex_to_bin(srec, address_32);
			sum = find_address_sum(srec, address_32);
			break;
		// 24-bit Termination
		case s8:
			s.start_address = ascii_hex_to_bin(srec, address_24);
			sum = find_address_sum(srec, address_24);
			break;
		// 16-bit Termination
		case s9:
			s.start_address = ascii_hex_to_bin(srec, address_16);
			sum = find_address_sum(srec, address_16);
			break;
		default:
			serial_print("Error: Invalid S-Record Type\n\rReturning unparsed s-record");
			return s;
	}

	if (s.type == s1 || s.type == s2 || s.type == s3)
	{
		// Set the data
		for (int i = 0; i < s.data_length; i++)
		{
			s.data[i] = (unsigned char)ascii_hex_to_bin(srec, byte_length);
			sum = sum + (int)s.data[i];
			srec = srec + byte_length;
		}
	}

	s.checksum = (unsigned char)ascii_hex_to_bin(srec, checksum_length);
	if (verify_checksum(sum, s.checksum) == false)
	{
		serial_print("Error: Checksum Failed\n\r");
	}
	return s;
}

srecord_type get_srec_type(char* str)
{
	char* buffer = str; //NOLINT
	buffer[2] = '\0';
	for (int i = 0; i < 2; i++)
	{
		if (cmp_str(buffer, srec_map[i].srec_type_str))
		{
			return srec_map[i].srec_type;
		}
	}
	serial_print("Invalid Srecord type, defaulting to s0");
	return s0;
}

uint32_t write_srecord(char* srec)
{
	char* ptr = &srec[0];

	while (true)
	{
		srecord s = parse_srec_line(ptr);
		ptr = ptr + s.size;

		if (s.type == s1 || s.type == s2 || s.type == s3)
		{
			serial_print("Writing to Memory...");
			// Write the data to memory
			for (int i = 0; i < s.data_length; i++)
			{
				write_memory(s.address, s.data[i]);
			}
		}
		else if (s.type == s7 || s.type == s8 || s.type == s9)
		{
			serial_print("Termination record found...");
			return s.start_address;
		}
		else if (s.type == s0) {}
		else
		{
			serial_print("Error: S-Record Parse Failed :(\n\r");
			break;
		}
	}
	serial_print("Error: No Termination Record\n\rDefault Start Address now at main");
	return (uint32_t) &main;
}

int find_address_sum(char* str, const int length)
{
	int sum = 0;
	for (int i = 0; i < length; i += 2)
	{
		sum += (int) ascii_hex_to_bin(str, byte_length);
		str += 2;
	}
	return sum;
}

bool verify_checksum(const int sum, const char checksum)
{
	return 0xff - (sum & 0xff) == checksum;
}