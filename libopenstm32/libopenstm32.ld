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
    FLASH0 (rx) : ORIGIN = 0x08000000, LENGTH = 256K
    SRAM0 (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
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
    } > FLASH0

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
	 PROVIDE (__bss_start__ = .);
	 *(.bss)
	 *(.bss*)
	 *(COMMON)
	 . = ALIGN(4);
	 PROVIDE (_ebss = .);
	 PROVIDE (__bss_end__ = .);
	 end = ALIGN(0x8);
	 _end = ALIGN(0x8);
	 __end = ALIGN(0x8);
    } > SRAM0
      
}
