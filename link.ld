OUTPUT_FORMAT("binary")
SECTIONS
{   
    . = 0x100000;
    
    .bootloader : ALIGN(1K)
    {
        *(.bootloader .bootloader*)
    }
    
    .auxFunctions : ALIGN(1K)
    {
    	misc_tools.o(.auxFunctions)
    	*(.auxFunctions .auxFunctions.*)
    }
    
    .text : ALIGN(4K)
    {
    	*(.text .text*)
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
    

    end = .;
}
