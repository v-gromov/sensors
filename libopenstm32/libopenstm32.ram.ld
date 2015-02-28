/******************************************************************************
 *
 *  sam3u_flash.ld - linker script for nutos on cotrex-m3 sam3u-ek
 *
 *****************************************************************************/

ENTRY(Reset_Handler)
STARTUP(init.o)
SEARCH_DIR(.)
OUTPUT_ARCH(arm)

 
MEMORY
{
    SRAM0 (rwx) : ORIGIN = 0x20001000, LENGTH = 60K
}

SECTIONS
{
    .text :
    {
	. = ALIGN(4);
        KEEP(*(.isr_vector))
        *(.text*)
        *(.rodata*)
	. = ALIGN(4);
        _etext = .;
    } > SRAM0

    .data : AT (ADDR(.text) + SIZEOF(.text))
    {
	. = ALIGN(4);
        _data = .;
        *(vtable)
        *(.data*)
	. = ALIGN(4);
        _edata = .;
    } > SRAM0

    .bss :
    {
	 PROVIDE (_bss = .);
	 *(.bss)
	 *(.bss*)
	 *(COMMON)
	 . = ALIGN(4);
	 PROVIDE (_ebss = .);
	 . += 0x400;
	 PROVIDE (__heap_start = .);
    } > SRAM0
}
end = .;
