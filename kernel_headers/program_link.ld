OUTPUT_FORMAT("binary")
SECTIONS
{   
    . = 0x50000;
    
    .system : ALIGN(1K)
    {
    	system.o(.system_program)
    	*(.system_program .system_program.*)
    }
    
    .text : ALIGN(1K)
    {
    	*(.text .text*)
    }
    
    .rodata : ALIGN(1K)
    {
        *(.rodata .rodata.*)
    }

    .data : ALIGN(1K)
    {
        *(.data .data.*)
    }

    .bss : ALIGN(1K)
    {
        *(.bss .bss.*)
    }
    

    end = .;
}
