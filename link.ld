ENTRY(start)
OUTPUT_FORMAT(binary)
SECTIONS {
    /DISCARD/ :
    {
        *(.note*);
        *(.iplt*);
        *(.igot*);
        *(.rel*);
        *(.comment);
    }
    . = 0x7c00;
    /* Bootloader */
    .boot :
	{
		*(.bootloader)
	}

    /* Code */
    .text :
	{
		*(.text)
	}

	/* Read-only data. */
	.rodata :
	{
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data :
	{
		*(.data)
	}

	/* Read-write data (uninitialized) and stack */
	.bss :
	{
		*(COMMON)
		*(.bss)
	}
}