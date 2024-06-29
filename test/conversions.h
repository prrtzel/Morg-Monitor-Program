#pragma once
#ifndef CONVERSIONS_H
#define CONVERSIONS_H
#include <stdint.h>

extern uint32_t ascii_hex_to_bin(const char* ascii_buffer, const int length);
extern unsigned char ascii_decimal_to_bin(const char ascii_char);
extern void binary_to_ascii_hex(const uint32_t binary_value, char* str_buffer, const unsigned char length);
#endif
