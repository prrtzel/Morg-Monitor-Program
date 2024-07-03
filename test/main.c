#include "morglib.h"
#include "morgio.h"

int main(void)
{
	while (!exit_code)
	{
		get_input();
		parse_cmd();
	}
	return 0;
}
