/* linker script to link the C kernel and the assembly
 multiboot file */

ENTRY(start)

SECTIONS
{

    /* start at 1MiB */
    . = 1M;

    .text BLOCK(4K) : ALIGN(4K)
    {
        /* put our multiboot section here */
        *(.multiboot)

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
		*(COMMON)
		*(.bss)
	}
}