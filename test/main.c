#include "morglib.h"
#include "morgio.h"

int main(void)
{
	serial_print("Morg Monitor Program\n\r"
  "Distribute at your own risk!\n\r"
  "ABSOLUTELY NO REFUNDS OR WARRANTY AND STUFF\n\r"
  "ver 4.0\n\r"
  "Type 'help' for a list of commands\n\r\n\r");

	while (!exit_code)
	{
		get_input();
		parse_cmd();
	}
	return 0;
}
