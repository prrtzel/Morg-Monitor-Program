#include "conversions.h"
#include "morgio.h"
#include "registers.h"
#include "morglib.h"

const struct {
	const char* str;
	registers reg;
} register_map[] = {
	{ "d0", d0 },
	{ "d1", d1 },
	{ "d2", d2 },
	{ "d3", d3 },
	{ "d4", d4 },
	{ "d5", d5 },
	{ "d6", d6 },
	{ "d7", d7 },
	{ "a0", a0 },
	{ "a1", a1 },
	{ "a2", a2 },
	{ "a3", a3 },
	{ "a4", a4 },
	{ "a5", a5 },
	{ "a6", a6 },
	{ "a7", a7 }
};

registers get_register(const char* str) {
	for (int i = 0; i < 16; i++) {
		if (cmp_str(str, register_map[i].str)) {
			return register_map[i].reg;
		}
	}
	serial_print("Error: No such register, defaulting to d0\n\r");
	return d0;
}

static uint32_t register_result = 0;
void read_register(registers reg) {
	register_result = 0;
	switch (reg) {
	case d0:
		__asm("move.l %d0, register_result");
		break;
	case d1:
		__asm("move.l %d1, register_result");
		break;
	case d2:
		__asm("move.l %d2, register_result");
		break;
	case d3:
		__asm("move.l %d3, register_result");
		break;
	case d4:
		__asm("move.l %d4, register_result");
		break;
	case d5:
		__asm("move.l %d5, register_result");
		break;
	case d6:
		__asm("move.l %d6, register_result");
		break;
	case d7:
		__asm("move.l %d7, register_result");
		break;
	case a0:
		__asm("move.l %a0, register_result");
		break;
	case a1:
		__asm("move.l %a1, register_result");
		break;
	case a2:
		__asm("move.l %a2, register_result");
		break;
	case a3:
		__asm("move.l %a3, register_result");
		break;
	case a4:
		__asm("move.l %a4, register_result");
		break;
	case a5:
		__asm("move.l %a5, register_result");
		break;
	case a6:
		__asm("move.l %a6, register_result");
		break;
	case a7:
		__asm("move.l %a7, register_result");
		break;
	}

	binary_to_ascii_hex(register_result, output_buffer, hex_long_length);
	serial_print(output_buffer);
	serial_print("\n\r");
}

static uint32_t reg_write_value = 0;
void write_register(registers reg, uint32_t data) {
	reg_write_value = data;
	switch (reg) {
	case d0:
		__asm("move.l reg_write_value, %d0");
		break;
	case d1:
		__asm("move.l reg_write_value, %d1");
		break;
	case d2:
		__asm("move.l reg_write_value, %d2");
		break;
	case d3:
		__asm("move.l reg_write_value, %d3");
		break;
	case d4:
		__asm("move.l reg_write_value, %d4");
		break;
	case d5:
		__asm("move.l reg_write_value, %d5");
		break;
	case d6:
		__asm("move.l reg_write_value, %d6");
		break;
	case d7:
		__asm("move.l reg_write_value, %d7");
		break;
	case a0:
		__asm("move.l reg_write_value, %a0");
		break;
	case a1:
		__asm("move.l reg_write_value, %a1");
		break;
	case a2:
		__asm("move.l reg_write_value, %a2");
		break;
	case a3:
		__asm("move.l reg_write_value, %a3");
		break;
	case a4:
		__asm("move.l reg_write_value, %a4");
		break;
	case a5:
		__asm("move.l reg_write_value, %a5");
		break;
	case a6:
		__asm("move.l reg_write_value, %a6");
		break;
	case a7:
		__asm("move.l reg_write_value, %a7");
		break;
	}
}
