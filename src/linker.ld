OUTPUT_FORMAT("binary")
ENTRY(main)
SECTIONS {
    . = 0x1000; /* Start address for the kernel */
    .text : { *(.text) }    /* Place all .text sections here */
    .data : { *(.data) }    /* Place all .data sections here */
    .bss  : { *(.bss) }     /* Place all .bss sections here */
}