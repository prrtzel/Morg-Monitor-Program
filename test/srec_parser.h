/* S-Record Parser
 * Morg only uses S0, S1, and S9 records
 * Will implement support for others if needed
 */
#ifndef SREC_PARSER_H
#define SREC_PARSER_H
#include <stdint.h>
#include <stdbool.h>

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


typedef enum srecord_type {
	s0,
	s1,
	s2,
	s3,
	s5,
	s7,
	s8,
	s9
}srecord_type;

const struct 
{
	const char* srec_type_str;
	srecord_type srec_type;
} srec_map[] = {
	{"S0", s0},
	{"S1", s1},
	{"S2", s2},
	{"S3", s3},
	{"S5", s5},
	{"S7", s7},
	{"S8", s8},
	{"S9", s9}
};

srecord_type get_srec_type(const char* str);

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

int find_address_sum(char* str, const int length);
bool verify_checksum(const int sum, const char checksum);
srecord parse_srec_line(char* srec);

//only public function
extern uint32_t write_srecord(char* srec);
#endif
