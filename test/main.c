#include "commands.h"
#include "morgio.h"

int main(void)
{
	// Initialize DUART
	init_duart();

	serial_print("\n\rMorg Monitor Program\n\r"
  "Distribute at your own risk!\n\r"
  "ABSOLUTELY NO REFUNDS OR WARRANTY AND STUFF\n\r"
  "ver 4.0\n\r"
  "Type 'help' for a list of commands\n\r");

	while (!exit_code)
	{
		get_input();
		parse_cmd();
	}
	return 0;
}
