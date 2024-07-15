#include <stdint.h>
#include "conversions.h"
#include "morgio.h"
#include "mem.h"

void read_memory(uint32_t address) { //NOLINT
	char* address_pointer = (char*)address; //NOLINT
	binary_to_ascii_hex(*address_pointer, output_buffer, hex_byte_length);
	serial_print(output_buffer);
}

void mem_dump(uint32_t starting_address, uint32_t ending_address)
{
	char j = 1;
	for (uint32_t i = starting_address; i <= ending_address; i++)
	{
		read_memory(i);
		serial_print(" ");
		if (j == 16)
		{
			serial_print("\n\r");
			j = 0;
		}
		j++;
	}
	serial_print("\n\r\n\r");
}

void write_memory(uint32_t address, const char data) {
	char* address_pointer = (char*)address; //NOLINT
	*address_pointer = data;
}
