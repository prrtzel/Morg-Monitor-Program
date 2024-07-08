#include <stdarg.h>

#include "conversions.h"
#include "morgio.h"
#include "morglib.h"

int printf(const char* str, ...)
{
	// Initialize list ptr
	va_list ptr;
	va_start(ptr, str);

	// char array to store token
	char token[1000];

	// Index of where to store the characters of str in token
	int k = 0;

	// Parsing the formatted string
	for (int i = 0; str[i] != '\0'; i++)
	{
		token[k] = str[i];
		k++;

		if (str[i + 1] == '%' || str[i + 1] == '\0')
		{
			token[k] = '\0';
			k = 0;

			// If the first character in the token is not '%'
			if (token[0] != '%')
			{
				// Print whole token up to '%'
				// if the first character is not '%'
				serial_print(token);
			}
			// If the first character in the token is '%'
			else
			{
				int j = 1;
				char ch1 = 0;

				// Loop through all the numbers
				while ((ch1 = token[j]) < '9')
				{
					j++;
				}

				// Integers
				if (ch1 == 'i' || ch1 == 'd' || ch1 == 'u' || ch1 == 'h')
				{
					binary_to_ascii_hex(va_arg(ptr, int), output_buffer, 10);
					serial_print(output_buffer);
					clear_buffer(output_buffer, output_buffer_size);
				}
				// Characters
				else if (ch1 == 'c')
				{
					output_buffer[0] = va_arg(ptr, int);
					output_buffer[1] = '\0';
					serial_print(output_buffer);
					clear_buffer(output_buffer, output_buffer_size);
				}
				else if (ch1 == 'l')
				{
					char ch2 = token[2];

					// Long int
					if (ch2 == 'u' || ch2 == 'd' || ch2 == 'i')
					{
						binary_to_ascii_hex(va_arg(ptr, long), output_buffer, 10);
						serial_print(output_buffer);
						clear_buffer(output_buffer, output_buffer_size);
					}
				}
				// Strings
				else if (ch1 == 's')
				{
					serial_print(va_arg(ptr, char*));
				}
				else
				{
					serial_print(token);
				}
			}
		}
	}
	va_end(ptr);
	return 0;
}
