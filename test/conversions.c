#include "morgio.h"
#include "conversions.h"
#include <stdint.h>
#include "morglib.h"

extern uint32_t ascii_hex_to_bin(const char* ascii_buffer) {
    uint32_t result = 0;
    const int length = get_str_length(ascii_buffer);
    
    for (int i = 0; i < length; i++) {
        unsigned char value = ascii_buffer[i];

        //convert the character in the string
        // if lowercase letter, subtract to convert to 0-16 range
        if (value >= 0x61 && value <= 0x66) {
            value = value - 0x57;
        }
        // if uppercase letter subtract amount to convert letter to number
        else if (value > 0x39) {
            value = value - 0x37;
        }
        // subtract 0x30 to extract just the digits
        else {
            value = value - 0x30;
        }
        if (value > 0x0F) {
            serial_print("Error: Conversion in ascii_hex_to_bin: value > 0x0F\n\r");
            return 0;
        }

        //shift it into the result
        const int shift_value = ((length - 1) * 4) - (i * 4);
        result = (value << shift_value) | result;
    }
    return result;
}

extern unsigned char ascii_decimal_to_bin(const char ascii_char) {
    unsigned char value = ascii_char;
    value = value - 0x30;
    return value;
}

extern void binary_to_ascii_hex(const uint32_t binary_value, char* str_buffer, const unsigned char length) {
    unsigned char shift_value = 0;

    //set binary value to array of characters
    for (int i = 0; i < length; i++) {
        //shift value decrements. Ex: 12, 8, 4, 0 if length = 4
        shift_value = ((length - 1) * 4) - (i * 4);
        str_buffer[i] = (binary_value & (0x0000000f << shift_value)) >> shift_value;  // NOLINT

        if (str_buffer[i] <= 0x09 && str_buffer[i] >= 0x00) {
            str_buffer[i] = str_buffer[i] | 0x30;
        }
        else if (str_buffer[i] <= 0x0f && str_buffer[i] >= 0x0a) {
            str_buffer[i] = str_buffer[i] - 0x09;
            str_buffer[i] = str_buffer[i] | 0x40;
        }
        else {
            serial_print("Error: Conversion in binary_to_ascii_hex\n\r");
            return;
        }
    }
    str_buffer[length] = 0;
}