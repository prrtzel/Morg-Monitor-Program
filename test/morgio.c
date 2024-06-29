#include "morgio.h"
#include "envctrl.h"

// serial print buffer
static char* str_ptr_buffer;
// get_char buffer
static char get_char_buffer;

char input_buffer[input_buffer_size];
char output_buffer[output_buffer_size];

void serial_print(const char* str_ptr) {
    str_ptr_buffer = (char*) str_ptr; //NOLINT
#ifdef SIM
    __asm("move.l  str_ptr_buffer, % a1\n\t\t"
          "move.b  #14, % d0\n\t\t"
          "trap    #15");
#endif  
#ifdef HW
#endif
}

extern char get_char(void) {
#ifdef SIM
    __asm("move.l	#5, % d0\n\t\t"
          "trap	    #15\n\t\t"
          "move.b % d1, get_char_buffer");
        return get_char_buffer;
#endif
#ifdef HARDWARE
#endif
}

extern void get_string(void) {
    int i = 0;
    char char_buffer = 1;

    clear_buffer(input_buffer, input_buffer_size);

    while (char_buffer != '\r') {
        if (i > input_buffer_size) {
            serial_print("Error: Input Overflow!");
            return;
        }

        char_buffer = get_char();

        if (char_buffer == backspace) {
            if (i != 0) {
                i--;
                input_buffer[i] = '\0';
                // Print the backspace
                serial_print("\b\0");
            }
        }
        else {
            input_buffer[i] = char_buffer;
            i++;
        }
    }
    serial_print("\n\r\0");
}

extern void clear_buffer(char* buffer, const int size) {
    for (int i = 0; i < size; i++) {
        *buffer++ = 0;
    }
}