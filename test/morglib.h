#ifndef MORGLIB_H
#define MORGLIB_H //NOLINT
#include <stdbool.h>
#include <stdint.h>
#include "commands.h"


/**
 * @brief clears a given array/buffer to 0
 *
 * @param buffer pointer to buffer thy wish to clear
 * @param size size of thy buffer (it does matter in this case)
 */
extern void clear_buffer(char* buffer, const int size);

int get_str_length(const char* string);

void split_str_cmd(const char* str, char split_string_array[num_of_cmds][arg_length]);

bool cmp_str(const char* str1, const char* str2);

extern void morg_memcpy(void* dest, void* src, uint32_t n);




#endif
