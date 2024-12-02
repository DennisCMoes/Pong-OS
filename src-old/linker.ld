OUTPUT_FORMAT("elf32-i386")
ENTRY(main)
SECTIONS {
    . = 0x001000; /* Start address for the kernel */
    .text : { *(.text) }    /* Place all .text sections here */
    .data : { *(.data) }    /* Place all .data sections here */
    .bss  : { *(.bss) }     /* Place all .bss sections here */
}