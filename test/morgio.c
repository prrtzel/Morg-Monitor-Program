#include "morgio.h"
#include "morglib.h"
#include "envctrl.h"
#include "morglib.h"

// Uart Registers
unsigned char* mr1_a = (unsigned char*)0x00100000 + 1; // Mode Register 1
unsigned char* mr2_a = (unsigned char*)0x00100000 + 1; // Mode Register 2
unsigned char* sr_a =  (unsigned char*)0x00100000 + 3; // Status Register
unsigned char* csr_a = (unsigned char*)0x00100000 + 3; // Clock Select Register
unsigned char* cr_a =  (unsigned char*)0x00100000 + 5; // Command Register
unsigned char* thr_a = (unsigned char*)0x00100000 + 7; // Transfer Holding Register
unsigned char* rhr_a = (unsigned char*)0x00100000 + 7; // Receive Holding Register
unsigned char* acr =   (unsigned char*)0x00100000 + 9; // Auxiliary Control Register
unsigned char* imr =   (unsigned char*)0x00100000 + 11;// Interrupt Mask Register

const char rx_rdy = 0;
const char tx_rdy = 2;
const unsigned char baud_rate = 0xCC; // 19.2k


#ifdef SIM
// serial print buffer
static char* str_ptr_buffer;

// get_char buffer
static char get_char_buffer;

// putc buffer
static char putc_buffer;
#endif

char input_buffer[input_buffer_size];
char output_buffer[output_buffer_size];

void serial_print(const char* str_ptr) {
#ifdef SIM
    str_ptr_buffer = (char*)str_ptr; //NOLINT
    __asm("move.l  str_ptr_buffer, % a1\n\t\t"
          "move.b  #14, % d0\n\t\t"
          "trap    #15");
#endif  
#ifdef HW
	while(*str_ptr != '\0')
	{
        putc(*str_ptr);
        str_ptr++;
	}
#endif
}


void putc(const char c) {
#ifdef SIM
    putc_buffer = c; //NOLINT
    __asm("move.l  putc_buffer, % a1\n\t\t"
          "move.b  #6, % d0\n\t\t"
          "trap    #15");
#endif  
#ifdef HW
    while ((*sr_a & (1 << tx_rdy)) == 0)
    {
	    // poll until tx_rdy bit is one
    }
    *thr_a = c;
#endif
}

extern char get_char(void) {
#ifdef SIM
    __asm("move.l	#5, % d0\n\t\t"
          "trap	    #15\n\t\t"
          "move.b % d1, get_char_buffer");
    return get_char_buffer;
#endif
#ifdef HW
    while ((*sr_a & (1 << rx_rdy)) == 0)
    {
        // poll until tx_rdy bit is one
    }
    char c = (char) *rhr_a;
    putc(c);
    return c;
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
                // cursor is moved back and a space is printed to overwrite the character
                serial_print(" \b");
            }
        }
        else {
            input_buffer[i] = char_buffer;
            i++;
        }
    }
    i--;
    input_buffer[i] = '\0';
    serial_print("\n\r\0");
}

void get_input(void)
{
	serial_print("Morg>> ");
	get_string();
}

void init_duart(void)
{
    // Software Reset
    *cr_a = 0x50; // Reset Break Change Interrupt
    *cr_a = 0x40; // Reset Error Status
    *cr_a = 0x30; // Reset TxA
    *cr_a = 0x20; // Reset RxA
	*cr_a = 0x10; // Reset MRA Pointer

    // Initialization
    *imr = 0x00; // Disable IMR
    *acr = 0x80; // Select Baud Rate Table 2
    *csr_a = baud_rate; // Set Baud Rate
    *mr1_a = 0x13; // 8-bits, no parity, 1 stop bit

    // 0x47 for auto echo 0x07 for normal mode
    *mr2_a = 0x07;
    *cr_a = 0x05; // Enable Tx and Rx
}