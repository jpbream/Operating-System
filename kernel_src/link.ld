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

        start_ctors = .;
        KEEP(*(.init_array));
        KEEP(*(SORT(.init_array.*)));
        end_ctors = .;

        *(.data)

        start_dtors = .;
        KEEP(*(.fini_array));
        KEEP(*(SORT(.fini_array.*)));
        end_dtors = .;
    }

    .bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
}