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
	s7,
	s8,
	s9,
	error,
	num_of_types
}srecord_type;




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

srecord_type get_srec_type(char* str);
void parse_srec_line(char* srec, srecord* srec_struct);
char get_checksum(char* srec_line, const int length);
//public functions
extern uint32_t write_srecord(char* srec);
extern void run_srec(uint32_t start_address);

#endif
