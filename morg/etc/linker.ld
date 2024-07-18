ENTRY(Reset_Handler)
OUTPUT_ARCH(m68k)
OUTPUT_FORMAT(srec)

MEMORY
{
    ROM (rx)  : ORIGIN = 0x00000000, LENGTH = 500k
    RAM (rwx) : ORIGIN = 0x00080000, LENGTH = 128k
}


PROVIDE(end_stack = 0x0009FFF0); 

SECTIONS
{
    .isr_vec (READONLY) :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vec))
        . = ALIGN(4);
    }> ROM

    .text (READONLY) :
    {
        . = ALIGN(4);
        *(.text)
        end_text = .;
    }> ROM

    .data :
    { 
        . = ALIGN(4);
        start_data = .; 
        *(.data);
        end_data = .;  
    } > RAM AT > ROM

    .bss :
    {
        . = ALIGN(4);
        start_bss = .;
        *(.bss)
        end_bss = .;
    }> RAM
}