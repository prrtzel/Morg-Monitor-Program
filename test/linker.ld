ENTRY(main)
OUTPUT_ARCH(m68k)
OUTPUT_FORMAT(srec)

MEMORY
{
    ROM (rx)  : ORIGIN = 0x00000000, LENGTH = 500K
    RAM (rwx) : ORIGIN = 0x00100000, LENGTH = 128K
}


PROVIDE(end_stack = 0x00100000 + 0x0001FFF0); 

SECTIONS
{
    .isr_vec :
    {
        . = ALIGN(4);
        *(.isr_vec)
        . = ALIGN(4);
    }> ROM

    .text :
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