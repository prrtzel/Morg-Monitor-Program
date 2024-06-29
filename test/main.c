#include "conversions.h"
#include "morgio.h"

int main(void)
{
	get_string();
	uint32_t var = ascii_hex_to_bin(input_buffer, 8);
	binary_to_ascii_hex(var, output_buffer, 8);
	serial_print(output_buffer);
	return 0;
}	