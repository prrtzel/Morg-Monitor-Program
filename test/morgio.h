/**
 * @author George Yacoubian
 * morgio.h
 * 
 * This is a very basic input/output library for
 * the morg monitor program and is not
 * to be used for a larger system.
 *
 **/

#pragma once
#ifndef MORGIO_H
#define MORGIO_H

// Backspace ASCII value
enum
{
	backspace = 0x08
};

// All buffer sizes
enum buffer_sizes {
	input_buffer_size = 128,
	output_buffer_size = 128
};

// Input and output buffers
extern char input_buffer[input_buffer_size];
extern char output_buffer[output_buffer_size];

/**
 * @brief This prints a string to the serial monitor
 * @param str_ptr Constant pointer to first char in the string
 */
extern void serial_print(const char* str_ptr);

/**
 * @brief Obtains a char from the serial monitor
 * @return char
 */
extern char get_char(void);

/**
 * @brief Obtains a string from the serial monitor and places it into
 * the input_buffer variable
 *
 */
extern void get_string(void);


void get_input(void);
#endif
