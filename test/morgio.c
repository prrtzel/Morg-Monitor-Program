#include "morgio.h"

static char* str_ptr_buffer;
void serial_print(const char* str_ptr) {
    str_ptr_buffer = (char*)str_ptr;
    __asm("move.l  str_ptr_buffer, % a1\n\t\t"
          "move.b  #14, % d0\n\t\t"
          "trap    #15");

}
