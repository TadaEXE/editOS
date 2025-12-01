ENTRY(_start)

SECTIONS
{
    . = 1M;

    .multiboot2 :
    {
        KEEP(*(.multiboot2))
    }

    .text :
    {
        *(.text*)
    }

    .rodata :
    {
        *(.rodata*)
    }

    .data :
    {
        *(.data*)
    }

    .bss :
    {
        *(.bss*)
        *(COMMON)
    }

    . = ALIGN(4K);
    _ld_kernel_end_addr = .;
}

