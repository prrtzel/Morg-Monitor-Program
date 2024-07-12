ENTRY(Reset_Handler)
OUTPUT_ARCH(m68k)
OUTPUT_FORMAT(srec)

MEMORY
{
    ROM (rx)  : ORIGIN = 0x00000000, LENGTH = 500k
    RAM (rwx) : ORIGIN = 0x00080000, LENGTH = 64k
}


PROVIDE(end_stack = 0x0008FF00); 

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
        . = ALIGN(4);
    }> ROM

    .data :
    { 
        . = ALIGN(4);
        start_data = .; 
        *(.data);
        end_data = .;  
        . = ALIGN(4);
    } > RAM AT > ROM

    .bss :
    {
        . = ALIGN(4);
        start_bss = .;
        *(.bss)
        end_bss = .;
        . = ALIGN(4);
    }> RAM
}