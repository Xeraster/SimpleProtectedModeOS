OUTPUT_FORMAT("elf32-i386")
ENTRY(begin)
SECTIONS
{
    . = 0x7e00;

    .text BLOCK(8K) : ALIGN(4K)
    {
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(.bss)
    }

    end = .;
}
