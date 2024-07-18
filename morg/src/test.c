#include "headers/morgio.h"

int main()
{
	init_duart();
	serial_print("Morg Testing in Progress\n\r");
	while(1)
	{
		get_string();
		serial_print(input_buffer);
	}
	return 0;
}
