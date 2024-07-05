#include "srec_parser.h"
#include "conversions.h"
#include "main.h"
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

	s.size = (char)type_length + byte_count_length + (s.byte_count * 2);
	int sum = s.byte_count;

	// Set the Address and Data Length
	switch (s.type)
	{
		case error:
			morg_memcpy(srec_struct, &s, sizeof(srecord));
			return;
		// 16-bit Header
		case s0:
			morg_memcpy(srec_struct, &s, sizeof(srecord));
			return;
		// 16-bit Data
		case s1:
			s.data_length = (s.byte_count - 3) * 2;
			s.address = ascii_hex_to_bin(srec, address_16);
			sum = find_address_sum(srec, address_16);
			srec = srec + address_16;
			break;
		// 24-bit Data
		case s2:
			s.data_length = (s.byte_count - 4) * 2;
			s.address = ascii_hex_to_bin(srec, address_24);
			sum = find_address_sum(srec, address_24);
			srec = srec + address_24;
			break;
		// 32-bit Data
		case s3:
			s.data_length = (s.byte_count - 5) * 2;
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
	}

	if (s.type == s1 || s.type == s2 || s.type == s3)
	{
		// Set the data
		int j = 0;
		for (int i = 0; i < s.data_length; i += 2)
		{
			s.data[j] = (char) ascii_hex_to_bin(&srec[i], byte_length);
			sum = sum + (int) s.data[j];
			j++;
		}
#ifdef DBUG
		serial_print(s.data);
		serial_print("\n\r");
#endif
	}
	srec = srec + s.data_length;
	s.checksum = (unsigned char)ascii_hex_to_bin(srec, checksum_length);

#ifdef DBUG
	char buffer[3];
	buffer[0] = srec[0];
	buffer[1] = srec[1];
	buffer[2] = '\0';
	serial_print("Checksum: ");
	serial_print(buffer);
	serial_print("\n\r");

	char buf2[3];
	binary_to_ascii_hex(sum, buf2, 2);
	buf2[2] = '\0';
	serial_print("Sum: ");
	serial_print(buf2);
	serial_print("\n\r");
#endif


	if (verify_checksum(sum, s.checksum) == false)
	{
		serial_print("Error: Checksum Failed\n\r");
		s.type = error;
	}
	morg_memcpy(srec_struct, &s, sizeof(srecord));
}

srecord_type get_srec_type(char* str)
{
	char buffer[3];
	buffer[0] = str[0];
	buffer[1] = str[1];
	buffer[2] = '\0';

#ifdef DBUG
	serial_print("get_srec_type buffer: ");
	serial_print(buffer);
	serial_print("\n\r");
#endif

	for (int i = 0; i < sizeof(srecord_type); i++)
	{
		if (cmp_str(buffer, srec_map[i].srec_type_str))
		{
#ifdef DBUG
			serial_print("Type: ");
			serial_print(srec_map[i].srec_type_str);
			serial_print("\n\r");
#endif
			return srec_map[i].srec_type;
		}
	}
	serial_print("Invalid Srecord type\n\r");
	return error;
}

uint32_t write_srecord(char* srec)
{
	char* ptr = &srec[0];

	while (true)
	{
		srecord s;
		parse_srec_line(ptr, &s);
		if (s.type == error) { break; }
		ptr = ptr + s.size;

		if (s.type == s1 || s.type == s2 || s.type == s3)
		{
			serial_print("Writing to Memory...\n\r");
			// Write the data to memory
			for (int i = 0; i < s.data_length; i++)
			{
				write_memory(s.address, s.data[i]);
			}
		}
		else if (s.type == s7 || s.type == s8 || s.type == s9)
		{
			serial_print("Termination record found...\n\r");
			return s.start_address;
		}
		else if (s.type == s0) {}
		else
		{
			serial_print("Error: No type record found.\n\r");
			break;
		}

	}
	serial_print("Error: Parse Failed. Default Start Address now at 0\n\r");
	return 0;
}

int find_address_sum(char* str, const int length)
{
	int sum = 0;
	for (int i = 0; i < length; i += 2)
	{
		sum += (int) ascii_hex_to_bin(str, byte_length);
		str += byte_length;
	}
	return sum;
}

bool verify_checksum(const int sum, const char checksum)
{
#ifdef DBUG
	int csum = 0;
	csum = 0xff - (sum & 0xff);
	char buf2[3];
	binary_to_ascii_hex(csum, buf2, 2);
	buf2[2] = '\0';
	serial_print("Calculated Checksum: ");
	serial_print(buf2);
	serial_print("\n\r");
#endif
	return 0xff - (sum & 0xff) == checksum;
}