#include "morgio.h"

int main()
{
	init_duart();
while (1)
{
	char c = get_char();
	putc(c);
}
	return 0;
}
