#ifndef SREC_PARSER_H
#define SREC_PARSER_H
#include <stdint.h>

//--------------------------------------------------------
// Public Functions / Variables
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

/**
 * @name get_srec_type
 * @brief Get the S-Record type from the S-Record string
 * @param str 
 * @return 
 */
srecord_type get_srec_type(char* str);

/**
 * @name parse_srec_line
 * @brief Parse an S-Record line into an S-Record struct
 * @param srec srecord string
 * @param srec_struct srec struct buffer to store output
 */
void parse_srec_line(char* srec, srecord* srec_struct);

/**
 * @name get_checksum
 * @brief Calculate the checksum of an S-Record line
 * @param srec_line S-Record line string
 * @param length Length of the S-Record line
 * @return checksum
 */
char get_checksum(char* srec_line, const int length);

//--------------------------------------------------------
// Public Functions

/**
 * @name write_srecord
 * @brief Write an S-Record to memory
 * @param srec srecord string
 * @return start address of the S-Record program
 */
uint32_t write_srecord(char* srec);

/**
 * @name run_srec
 * @brief Run an S-Record program
 * @param start_address start address of the S-Record program
 */
void run_srec(uint32_t start_address);

void disp_srec_loading_bar(int num_of_records, int num_of_srec_written);

#endif
