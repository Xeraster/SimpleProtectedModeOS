OUTPUT_FORMAT("binary")
SECTIONS
{
    . = 0x7c00;
    
    .text : ALIGN(1K)
    {
        bootloader.o(.text)
        *(.text .text.*)
    }

    .rodata : ALIGN(4K)
    {
        *(.rodata .rodata.*)
    }

    .data : ALIGN(4K)
    {
        *(.data .data.*)
    }

    .bss : ALIGN(4K)
    {
        *(.bss .bss.*)
    }

    .auxFunctions : ALIGN(1K)
    {
        
    }

    end = .;
}
