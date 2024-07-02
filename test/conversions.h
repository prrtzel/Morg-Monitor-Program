#pragma once
#ifndef CONVERSIONS_H
#define CONVERSIONS_H
#include <stdint.h>

enum ascii_bin_lengths
{
	hex_byte_length = 2,
	hex_word_length = 4,
	hex_long_length = 8
};

/**
 * @brief Converts ascii hex string to binary 
 * @param ascii_buffer 
 * @return uint32_t
 */
extern uint32_t ascii_hex_to_bin(const char* ascii_buffer);

/**
 * @brief Converts ascii decimal string to binary
 *
 * @param ascii_char
 * @return char
 */
extern unsigned char ascii_decimal_to_bin(const char ascii_char);

/**
 * @brief Converts a binary number to ascii hex
 *
 * @param binary_value raw value
 * @param str_buffer buffer to place the string in
 * @param length number of characters to produce
 */
extern void binary_to_ascii_hex(const uint32_t binary_value, char* str_buffer, const unsigned char length);
#endif
