#include "morglib.h"
#include <stdbool.h>
#include <stdint.h>
#include "conversions.h"
#include "morgio.h"

void clear_buffer(char* buffer, const int size) {
	for (int i = 0; i < size; i++) {
		*buffer++ = 0;
	}
}

int get_str_length(const char* string) {
	int i = 0;
	while (string[i] != '\0') {
		i++;
	}
	return i;
}

void split_str(const char* str, char split_string_array[num_of_cmds][arg_length]) {
	const int str_length = get_str_length(str);

	// counters and index vars
	int j = 0;
	int ctr = 0;

	// loop through each char in input string
	for (int i = 0; i <= str_length; i++)
	{
		if (str[i] == ' ' || str[i] == '\0')
		{
			split_string_array[ctr][j] = '\0';
			ctr++; // move on to the next word
			j = 0;
		}
		else
		{
			split_string_array[ctr][j] = str[i];
			j++;
		}
	}
}

bool cmp_str(const char* str1, const char* str2) {
	while (*str1 != '\0' || *str2 != '\0') {
		if (*str1 == *str2) {
			str1++;
			str2++;
		}
		else if ((*str1 != '\0' && *str2 == '\0') ||
			(*str1 == '\0' && *str2 != '\0') ||
			(*str1 != *str2)) {
			return false;
		}
	}
	return true;
}

void morg_memcpy(void *dest, void* src, uint32_t n)
{
	char* c_dest = (char*)dest;
	char* c_src = (char*)src;

	for (uint32_t i = 0; i < n; i++)
	{
		c_dest[i] = c_src[i];
	}
}

